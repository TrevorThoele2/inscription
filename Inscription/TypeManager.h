#pragma once

#include "ObjectTracker.h"
#include "TypeTracker.h"
#include "PolymorphicManager.h"
#include "TypeRegistrationContext.h"
#include "ShouldTrack.h"

namespace Inscription
{
    class TypeManagerBase
    {
    public:
        virtual ~TypeManagerBase() = 0;

        virtual bool TrackObjects(bool set = true) = 0;
    protected:
        TypeManagerBase() = default;
    };

    template<class DerivedFormat>
    class TypeManager final : public TypeManagerBase
    {
    public:
        using TypeRegistrationContext = TypeRegistrationContext<DerivedFormat>;
    public:
        TypeManager(DerivedFormat& format);
        TypeManager(DerivedFormat& format, TypeRegistrationContext typeRegistrationContext);
        TypeManager(TypeManager&& arg, DerivedFormat& format) noexcept;
        TypeManager& operator=(TypeManager&& arg) noexcept;
    public:
        // Will remove all of the target's tracking history
        // For this reason, this should probably be called at the beginning of the target's lifetime
        void CopyTrackersTo(DerivedFormat& target) const;
        // Will remove all of the target's tracking history
        // For this reason, this should probably be called at the beginning of the target's lifetime
        // Be sure that this format will no longer need to handle pointers through tracking when calling this
        void MoveTrackersTo(DerivedFormat& target);

        void ClearTrackers();
    public:
        template<class T>
        std::optional<TrackingID> AttemptTrackObject(T* arg);
        template<class T>
        std::optional<TrackingID> AttemptTrackObject(T* arg, TrackingID id);
        template<class T>
        void AttemptReplaceTrackedObject(T& here, T& newObject);
        void AttemptReplaceTrackedObject(void* here, void* newObject);
        bool TrackObjects(bool set = true) override;

        void TrackSavedConstruction(TrackingID trackingID);
        [[nodiscard]] bool HasSavedConstruction(TrackingID trackingID) const;

        [[nodiscard]] void* FindObject(TrackingID id);
        [[nodiscard]] std::optional<TrackingID> FindObjectID(void* object);
    public:
        template<class T>
        TrackingID AddType();
        TrackingID AddType(const std::type_index& type);
        TrackingID AddType(const std::type_index& type, TrackingID id);

        [[nodiscard]] std::optional<std::type_index> FindType(TrackingID id) const;
        template<class T>
        [[nodiscard]] std::optional<TrackingID> FindTypeID() const;
        [[nodiscard]] std::optional<TrackingID> FindTypeID(const std::type_index& type) const;
    public:
        template<class T>
        void PolymorphicSave(const T* object);
        void PolymorphicSave(void* object, const std::type_index& typeIndex);

        void ConstructPolymorphic(void*& storage, const std::type_index& typeIndex);
        void* CreatePolymorphicStorage(const std::type_index& typeIndex);

        [[nodiscard]] std::type_index PolymorphicTypeIndexFor(const Type& type);
        [[nodiscard]] Type PolymorphicOutputTypeFor(const std::type_index& typeIndex);

        template<class T>
        void RegisterType();
    private:
        DerivedFormat* format = nullptr;
    private:
        ObjectTracker objectTracker;
        TypeTracker typeTracker;
        PolymorphicManager<DerivedFormat> polymorphicManager;
    private:
        TypeRegistrationContext typeRegistrationContext;

        template<class T, std::enable_if_t<!std::is_abstract_v<T> && std::is_polymorphic_v<T>, int> = 0>
        void RegisterTypeImpl();
        template<class T, std::enable_if_t<std::is_abstract_v<T> || !std::is_polymorphic_v<T>, int> = 0>
        void RegisterTypeImpl();
    };

    template<class DerivedFormat>
    TypeManager<DerivedFormat>::TypeManager(DerivedFormat& format) :
        format(&format)
    {}

    template<class DerivedFormat>
    TypeManager<DerivedFormat>::TypeManager(DerivedFormat& format, TypeRegistrationContext typeRegistrationContext) :
        format(&format), typeRegistrationContext(typeRegistrationContext)
    {}

    template<class DerivedFormat>
    TypeManager<DerivedFormat>::TypeManager(TypeManager&& arg, DerivedFormat& format) noexcept :
        format(&format),
        objectTracker(std::move(arg.objectTracker)),
        typeTracker(std::move(arg.typeTracker)),
        polymorphicManager(std::move(arg.polymorphicManager)),
        typeRegistrationContext(std::move(arg.typeRegistrationContext))
    {}

    template<class DerivedFormat>
    TypeManager<DerivedFormat>& TypeManager<DerivedFormat>::operator=(TypeManager&& arg) noexcept
    {
        format = std::move(arg.format);
        objectTracker = std::move(arg.objectTracker);
        typeTracker = std::move(arg.typeTracker);
        polymorphicManager = std::move(arg.polymorphicManager);
        typeRegistrationContext = std::move(arg.typeRegistrationContext);
        return*this;
    }

    template<class DerivedFormat>
    void TypeManager<DerivedFormat>::CopyTrackersTo(DerivedFormat& target) const
    {
        target.objectTracker = objectTracker;
    }

    template<class DerivedFormat>
    void TypeManager<DerivedFormat>::MoveTrackersTo(DerivedFormat& target)
    {
        target.objectTracker = std::move(objectTracker);
    }

    template<class DerivedFormat>
    void TypeManager<DerivedFormat>::ClearTrackers()
    {
        objectTracker.Clear();
    }

    template<class DerivedFormat>
    template<class T>
    std::optional<TrackingID> TypeManager<DerivedFormat>::AttemptTrackObject(T* arg)
    {
        if (!should_track_v<T, DerivedFormat>)
            return {};

        return objectTracker.Add(arg);
    }

    template<class DerivedFormat>
    template<class T>
    std::optional<TrackingID> TypeManager<DerivedFormat>::AttemptTrackObject(T* arg, TrackingID id)
    {
        if (!should_track_v<T, DerivedFormat>)
            return {};

        return objectTracker.Add(arg, id);
    }

    template<class DerivedFormat>
    template<class T>
    void TypeManager<DerivedFormat>::AttemptReplaceTrackedObject(T& here, T& newObject)
    {
        if (!should_track_v<T, DerivedFormat>)
            return;

        objectTracker.ReplaceObject(&here, &newObject);
    }

    template<class DerivedFormat>
    void TypeManager<DerivedFormat>::AttemptReplaceTrackedObject(void* here, void* newObject)
    {
        objectTracker.ReplaceObject(here, newObject);
    }

    template<class DerivedFormat>
    bool TypeManager<DerivedFormat>::TrackObjects(bool set)
    {
        const auto isActive = objectTracker.IsActive();
        objectTracker.Activate(set);
        return isActive;
    }

    template<class DerivedFormat>
    void TypeManager<DerivedFormat>::TrackSavedConstruction(TrackingID trackingID)
    {
        objectTracker.SignalSavedConstruction(trackingID);
    }

    template<class DerivedFormat>
    bool TypeManager<DerivedFormat>::HasSavedConstruction(TrackingID trackingID) const
    {
        return objectTracker.HasSavedConstruction(trackingID);
    }

    template<class DerivedFormat>
    void* TypeManager<DerivedFormat>::FindObject(TrackingID id)
    {
        return objectTracker.FindObject(id);
    }

    template<class DerivedFormat>
    std::optional<TrackingID> TypeManager<DerivedFormat>::FindObjectID(void* object)
    {
        return objectTracker.FindID(object);
    }

    template<class DerivedFormat>
    template<class T>
    TrackingID TypeManager<DerivedFormat>::AddType()
    {
        return typeTracker.Add<T>();
    }

    template<class DerivedFormat>
    TrackingID TypeManager<DerivedFormat>::AddType(const std::type_index& type)
    {
        return typeTracker.Add(type);
    }

    template<class DerivedFormat>
    TrackingID TypeManager<DerivedFormat>::AddType(const std::type_index& type, TrackingID id)
    {
        return typeTracker.Add(type, id);
    }

    template<class DerivedFormat>
    std::optional<std::type_index> TypeManager<DerivedFormat>::FindType(TrackingID id) const
    {
        return typeTracker.FindType(id);
    }

    template<class DerivedFormat>
    template<class T>
    std::optional<TrackingID> TypeManager<DerivedFormat>::FindTypeID() const
    {
        return typeTracker.FindID<T>();
    }

    template<class DerivedFormat>
    std::optional<TrackingID> TypeManager<DerivedFormat>::FindTypeID(const std::type_index& type) const
    {
        return typeTracker.FindID(type);
    }

    template<class DerivedFormat>
    template<class T>
    void TypeManager<DerivedFormat>::PolymorphicSave(const T* object)
    {
        polymorphicManager.Save(object, *format);
    }

    template<class DerivedFormat>
    void TypeManager<DerivedFormat>::PolymorphicSave(void* object, const std::type_index& typeIndex)
    {
        polymorphicManager.Save(object, typeIndex, *format);
    }

    template<class DerivedFormat>
    void TypeManager<DerivedFormat>::ConstructPolymorphic(void*& storage, const std::type_index& typeIndex)
    {
        polymorphicManager.Construct(storage, typeIndex, *format);
    }

    template<class DerivedFormat>
    void* TypeManager<DerivedFormat>::CreatePolymorphicStorage(const std::type_index& typeIndex)
    {
        return polymorphicManager.CreateStorage(typeIndex);
    }

    template<class DerivedFormat>
    std::type_index TypeManager<DerivedFormat>::PolymorphicTypeIndexFor(const Type& type)
    {
        return polymorphicManager.TypeIndexFor(type);
    }

    template<class DerivedFormat>
    Type TypeManager<DerivedFormat>::PolymorphicOutputTypeFor(const std::type_index& typeIndex)
    {
        return polymorphicManager.OutputTypeFor(typeIndex);
    }

    template<class DerivedFormat>
    template<class T>
    void TypeManager<DerivedFormat>::RegisterType()
    {
        static_assert(std::is_class_v<T>, "A registered type must be a class type.");

        typeRegistrationContext.template RegisterType<T>();
        RegisterTypeImpl<T>();
    }

    template<class DerivedFormat>
    template<class T, std::enable_if_t<!std::is_abstract_v<T> && std::is_polymorphic_v<T>, int>>
    void TypeManager<DerivedFormat>::RegisterTypeImpl()
    {
        polymorphicManager.template Register<T>(*format);
    }

    template<class DerivedFormat>
    template<class T, std::enable_if_t<std::is_abstract_v<T> || !std::is_polymorphic_v<T>, int>>
    void TypeManager<DerivedFormat>::RegisterTypeImpl()
    {}
}