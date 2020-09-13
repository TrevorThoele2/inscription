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

    template<class DerivedArchive>
    class TypeManager final : public TypeManagerBase
    {
    public:
        using TypeRegistrationContext = TypeRegistrationContext<DerivedArchive>;
    public:
        TypeManager(DerivedArchive& archive);
        TypeManager(DerivedArchive& archive, TypeRegistrationContext typeRegistrationContext);
        TypeManager(TypeManager&& arg, DerivedArchive& archive) noexcept;
        TypeManager& operator=(TypeManager&& arg) noexcept;
    public:
        // Will remove all of the target's tracking history
        // For this reason, this should probably be called at the beginning of the target's lifetime
        void CopyTrackersTo(DerivedArchive& target) const;
        // Will remove all of the target's tracking history
        // For this reason, this should probably be called at the beginning of the target's lifetime
        // Be sure that this archive will no longer need to handle pointers through tracking when calling this
        void MoveTrackersTo(DerivedArchive& target);

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
        DerivedArchive* archive = nullptr;
    private:
        ObjectTracker objectTracker;
        TypeTracker typeTracker;
        PolymorphicManager<DerivedArchive> polymorphicManager;
    private:
        TypeRegistrationContext typeRegistrationContext;

        template<class T, std::enable_if_t<!std::is_abstract_v<T> && std::is_polymorphic_v<T>, int> = 0>
        void RegisterTypeImpl();
        template<class T, std::enable_if_t<std::is_abstract_v<T> || !std::is_polymorphic_v<T>, int> = 0>
        void RegisterTypeImpl();
    };

    template<class DerivedArchive>
    TypeManager<DerivedArchive>::TypeManager(DerivedArchive& archive) :
        archive(&archive)
    {}

    template<class DerivedArchive>
    TypeManager<DerivedArchive>::TypeManager(DerivedArchive& archive, TypeRegistrationContext typeRegistrationContext) :
        archive(&archive), typeRegistrationContext(typeRegistrationContext)
    {}

    template<class DerivedArchive>
    TypeManager<DerivedArchive>::TypeManager(TypeManager&& arg, DerivedArchive& archive) noexcept :
        archive(&archive),
        objectTracker(std::move(arg.objectTracker)),
        typeTracker(std::move(arg.typeTracker)),
        polymorphicManager(std::move(arg.polymorphicManager)),
        typeRegistrationContext(std::move(arg.typeRegistrationContext))
    {}

    template<class DerivedArchive>
    TypeManager<DerivedArchive>& TypeManager<DerivedArchive>::operator=(TypeManager&& arg) noexcept
    {
        archive = std::move(arg.archive);
        objectTracker = std::move(arg.objectTracker);
        typeTracker = std::move(arg.typeTracker);
        polymorphicManager = std::move(arg.polymorphicManager);
        typeRegistrationContext = std::move(arg.typeRegistrationContext);
        return*this;
    }

    template<class DerivedArchive>
    void TypeManager<DerivedArchive>::CopyTrackersTo(DerivedArchive& target) const
    {
        target.objectTracker = objectTracker;
    }

    template<class DerivedArchive>
    void TypeManager<DerivedArchive>::MoveTrackersTo(DerivedArchive& target)
    {
        target.objectTracker = std::move(objectTracker);
    }

    template<class DerivedArchive>
    void TypeManager<DerivedArchive>::ClearTrackers()
    {
        objectTracker.Clear();
    }

    template<class DerivedArchive>
    template<class T>
    std::optional<TrackingID> TypeManager<DerivedArchive>::AttemptTrackObject(T* arg)
    {
        if (!should_track_v<T, DerivedArchive>)
            return {};

        return objectTracker.Add(arg);
    }

    template<class DerivedArchive>
    template<class T>
    std::optional<TrackingID> TypeManager<DerivedArchive>::AttemptTrackObject(T* arg, TrackingID id)
    {
        if (!should_track_v<T, DerivedArchive>)
            return {};

        return objectTracker.Add(arg, id);
    }

    template<class DerivedArchive>
    template<class T>
    void TypeManager<DerivedArchive>::AttemptReplaceTrackedObject(T& here, T& newObject)
    {
        if (!should_track_v<T, DerivedArchive>)
            return;

        objectTracker.ReplaceObject(&here, &newObject);
    }

    template<class DerivedArchive>
    void TypeManager<DerivedArchive>::AttemptReplaceTrackedObject(void* here, void* newObject)
    {
        objectTracker.ReplaceObject(here, newObject);
    }

    template<class DerivedArchive>
    bool TypeManager<DerivedArchive>::TrackObjects(bool set)
    {
        const auto isActive = objectTracker.IsActive();
        objectTracker.Activate(set);
        return isActive;
    }

    template<class DerivedArchive>
    void TypeManager<DerivedArchive>::TrackSavedConstruction(TrackingID trackingID)
    {
        objectTracker.SignalSavedConstruction(trackingID);
    }

    template<class DerivedArchive>
    bool TypeManager<DerivedArchive>::HasSavedConstruction(TrackingID trackingID) const
    {
        return objectTracker.HasSavedConstruction(trackingID);
    }

    template<class DerivedArchive>
    void* TypeManager<DerivedArchive>::FindObject(TrackingID id)
    {
        return objectTracker.FindObject(id);
    }

    template<class DerivedArchive>
    std::optional<TrackingID> TypeManager<DerivedArchive>::FindObjectID(void* object)
    {
        return objectTracker.FindID(object);
    }

    template<class DerivedArchive>
    template<class T>
    TrackingID TypeManager<DerivedArchive>::AddType()
    {
        return typeTracker.Add<T>();
    }

    template<class DerivedArchive>
    TrackingID TypeManager<DerivedArchive>::AddType(const std::type_index& type)
    {
        return typeTracker.Add(type);
    }

    template<class DerivedArchive>
    TrackingID TypeManager<DerivedArchive>::AddType(const std::type_index& type, TrackingID id)
    {
        return typeTracker.Add(type, id);
    }

    template<class DerivedArchive>
    std::optional<std::type_index> TypeManager<DerivedArchive>::FindType(TrackingID id) const
    {
        return typeTracker.FindType(id);
    }

    template<class DerivedArchive>
    template<class T>
    std::optional<TrackingID> TypeManager<DerivedArchive>::FindTypeID() const
    {
        return typeTracker.FindID<T>();
    }

    template<class DerivedArchive>
    std::optional<TrackingID> TypeManager<DerivedArchive>::FindTypeID(const std::type_index& type) const
    {
        return typeTracker.FindID(type);
    }

    template<class DerivedArchive>
    template<class T>
    void TypeManager<DerivedArchive>::PolymorphicSave(const T* object)
    {
        polymorphicManager.Save(object, *archive);
    }

    template<class DerivedArchive>
    void TypeManager<DerivedArchive>::PolymorphicSave(void* object, const std::type_index& typeIndex)
    {
        polymorphicManager.Save(object, typeIndex, *archive);
    }

    template<class DerivedArchive>
    void TypeManager<DerivedArchive>::ConstructPolymorphic(void*& storage, const std::type_index& typeIndex)
    {
        polymorphicManager.Construct(storage, typeIndex, *archive);
    }

    template<class DerivedArchive>
    void* TypeManager<DerivedArchive>::CreatePolymorphicStorage(const std::type_index& typeIndex)
    {
        return polymorphicManager.CreateStorage(typeIndex);
    }

    template<class DerivedArchive>
    std::type_index TypeManager<DerivedArchive>::PolymorphicTypeIndexFor(const Type& type)
    {
        return polymorphicManager.TypeIndexFor(type);
    }

    template<class DerivedArchive>
    Type TypeManager<DerivedArchive>::PolymorphicOutputTypeFor(const std::type_index& typeIndex)
    {
        return polymorphicManager.OutputTypeFor(typeIndex);
    }

    template<class DerivedArchive>
    template<class T>
    void TypeManager<DerivedArchive>::RegisterType()
    {
        static_assert(std::is_class_v<T>, "A registered type must be a class type.");

        typeRegistrationContext.template RegisterType<T>();
        RegisterTypeImpl<T>();
    }

    template<class DerivedArchive>
    template<class T, std::enable_if_t<!std::is_abstract_v<T> && std::is_polymorphic_v<T>, int>>
    void TypeManager<DerivedArchive>::RegisterTypeImpl()
    {
        polymorphicManager.template Register<T>(*archive);
    }

    template<class DerivedArchive>
    template<class T, std::enable_if_t<std::is_abstract_v<T> || !std::is_polymorphic_v<T>, int>>
    void TypeManager<DerivedArchive>::RegisterTypeImpl()
    {}
}