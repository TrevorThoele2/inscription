#pragma once

#include "ScribeBase.h"
#include "TrackingScribe.h"

#include "ObjectTrackingContext.h"
#include "SpecialObjectTrackingID.h"

#include "NumericScribe.h"
#include "StringScribe.h"

#include "BinaryArchive.h"

namespace Inscription
{
    template<class Object, class Archive>
    class CompositeScribe : public ScribeBase<Object, Archive>
    {
    private:
        using BaseT = ScribeBase<Object, Archive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        void Scriven(ObjectT& object, ArchiveT& archive) override;
    protected:
        CompositeScribe() = default;
        CompositeScribe(const CompositeScribe& arg) = default;

        using BaseT::ScrivenImplementation;
    private:
        static_assert(std::is_class_v<ObjectT>,
            "The Object given to a CompositeScribe was not a composite.");
    };

    template<class Object, class Archive>
    void CompositeScribe<Object, Archive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        ScrivenImplementation(object, archive);
    }

    template<class Object>
    class CompositeScribe<Object, BinaryArchive> : public TrackingScribe<Object, BinaryArchive>
    {
    private:
        using BaseT = TrackingScribe<Object, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        using BaseT::Scriven;
        virtual void Scriven(ObjectT*& object, ArchiveT& archive);
    protected:
        CompositeScribe() = default;
        CompositeScribe(const CompositeScribe& arg) = default;

        using BaseT::ScrivenImplementation;
    private:
        void Save(ObjectT*& object, ArchiveT& archive);
        void Load(ObjectT*& object, ArchiveT& archive);
    private:
        void SaveConstruction
        (
            ObjectT*& object,
            ArchiveT& archive,
            std::true_type
        );
        void SaveConstruction
        (
            ObjectT*& object,
            ArchiveT& archive,
            std::false_type
        );
        void LoadConstruction
        (
            ObjectT*& object,
            ArchiveT& archive,
            TrackingID objectID,
            std::true_type
        );
        void LoadConstruction
        (
            ObjectT*& object,
            ArchiveT& archive,
            TrackingID objectID,
            std::false_type
        );
    private:
        [[nodiscard]] Scribe<ObjectT, ArchiveT>& AsScribe()
        {
            return static_cast<Scribe<ObjectT, ArchiveT>&>(*this);
        }

        [[nodiscard]] const Scribe<ObjectT, ArchiveT>& AsScribe() const
        {
            return static_cast<const Scribe<ObjectT, ArchiveT>&>(*this);
        }
    private:
        static_assert(std::is_class_v<ObjectT>,
            "The Object given to a CompositeScribe was not a composite.");
    };

    template<class Object>
    void CompositeScribe<Object, BinaryArchive>::Scriven(ObjectT*& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            Save(object, archive);
        else
            Load(object, archive);
    }

    template<class Object>
    void CompositeScribe<Object, BinaryArchive>::Save(
        ObjectT*& object, ArchiveT& archive)
    {
        if (object == nullptr)
        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Inactive, archive);
            archive(SpecialObjectTrackingID::NULLPTR);
            return;
        }

        TrackingID objectID;

        auto shouldSaveConstructionObject = true;

        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Active, archive);
            auto foundObjectID = archive.objectTracker.FindID(object);
            if (foundObjectID.has_value())
            {
                objectID = *foundObjectID;
                shouldSaveConstructionObject = false;
            }
            else
                objectID = *archive.objectTracker.Add(object);
        }

        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Inactive, archive);
            archive(objectID);
        }

        if (!shouldSaveConstructionObject || archive.objectTracker.HasSavedConstruction(objectID))
            return;

        archive.objectTracker.SignalSavedConstruction(objectID);

        SaveConstruction
        (
            object,
            archive,
            std::bool_constant<std::is_polymorphic_v<ObjectT>>{}
        );
    }

    template<class Object>
    void CompositeScribe<Object, BinaryArchive>::Load(
        ObjectT*& object, ArchiveT& archive)
    {
        TrackingID objectID;

        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Inactive, archive);
            archive(objectID);
        }

        if (objectID == SpecialObjectTrackingID::NULLPTR)
        {
            object = nullptr;
            return;
        }

        auto foundObject = archive.objectTracker.FindObject(objectID);
        if (foundObject)
        {
            object = reinterpret_cast<ObjectT*>(foundObject);
            return;
        }

        LoadConstruction
        (
            object,
            archive,
            objectID,
            std::bool_constant<std::is_polymorphic_v<ObjectT>>{}
        );
    }

    template<class Object>
    void CompositeScribe<Object, BinaryArchive>::SaveConstruction(
        ObjectT*& object, ArchiveT& archive, std::true_type)
    {
        TrackingID typeID;
        bool needsType;
        auto typeIndex = std::type_index(typeid(*object));

        {
            auto foundTypeID = archive.typeTracker.FindID(typeIndex);
            needsType = !foundTypeID.has_value();
            if (foundTypeID.has_value())
                typeID = *foundTypeID;
            else
                typeID = archive.typeTracker.Add(typeIndex);
        }

        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Inactive, archive);
            archive(typeID);
            if (needsType)
            {
                auto type = archive.polymorphicManager.OutputTypeFor(typeIndex);
                archive(type);
            }
        }

        archive.polymorphicManager.Save(object, archive);
    }

    template<class Object>
    void CompositeScribe<Object, BinaryArchive>::SaveConstruction(
        ObjectT*& object, ArchiveT& archive, std::false_type)
    {
        Scriven(*object, archive);
    }

    template<class Object>
    void CompositeScribe<Object, BinaryArchive>::LoadConstruction(
        ObjectT*& object, ArchiveT& archive, TrackingID objectID, std::true_type)
    {
        TrackingID typeID;
        std::type_index typeIndex = typeid(void*);

        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Inactive, archive);
            archive(typeID);
            auto foundType = archive.typeTracker.FindType(typeID);
            if (!foundType.has_value())
            {
                Type type;
                archive(type);

                typeIndex = archive.polymorphicManager.TypeIndexFor(type);
                archive.typeTracker.Add(typeIndex, typeID);
            }
            else
                typeIndex = *foundType;
        }

        auto storage = archive.polymorphicManager.CreateStorage(typeIndex);
        archive.objectTracker.Add(storage, objectID);
        archive.polymorphicManager.Construct(storage, typeIndex, archive);
        object = reinterpret_cast<ObjectT*>(storage);
    }

    template<class Object>
    void CompositeScribe<Object, BinaryArchive>::LoadConstruction(
        ObjectT*& object, ArchiveT& archive, TrackingID objectID, std::false_type)
    {
        auto storage = reinterpret_cast<ObjectT*>(CreateStorage(sizeof(ObjectT)));
        Access::Construct(storage, archive, AsScribe());
        object = storage;
    }
}