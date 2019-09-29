#pragma once

#include "ScribeBase.h"

#include "SpecialObjectTrackingID.h"
#include "ObjectTrackingContext.h"

#include "PolymorphicManager.h"

#include "NumericScribe.h"
#include "StringScribe.h"
#include "Access.h"

#include "Storage.h"

namespace Inscription
{
    template<class Object, class Archive>
    class PointerScribe : public ScribeBase<Object, Archive>
    {
    private:
        using BaseT = ScribeBase<Object, Archive>;
    public:
        using typename BaseT::ObjectT;
        using typename BaseT::ArchiveT;

        using BaseT::Scriven;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    private:
        void SaveImplementation(ObjectT& object, ArchiveT& archive);
        void LoadImplementation(ObjectT& object, ArchiveT& archive);
    private:
        using BareObject = std::remove_pointer_t<Object>;
        using BareScribe = Scribe<BareObject, ArchiveT>;
        BareScribe bareScribe;
    private:
        constexpr static bool shouldConstructPolymorphically = std::is_polymorphic_v<BareObject>;

        void SaveConstruction(
            ObjectT& object,
            ArchiveT& archive,
            std::true_type);
        void SaveConstruction(
            ObjectT& object,
            ArchiveT& archive,
            std::false_type);
        void LoadConstruction(
            ObjectT& object,
            ArchiveT& archive,
            TrackingID objectID,
            std::true_type);
        void LoadConstruction(
            ObjectT& object,
            ArchiveT& archive,
            TrackingID objectID,
            std::false_type);
    private:
        static_assert(std::is_pointer_v<ObjectT>,
            "The Object given to a PointerScribe was not a pointer.");
    };

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::SaveImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (object == nullptr)
        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Inactive, archive);
            archive(SpecialObjectTrackingID::NULLPTR);
            return;
        }

        TrackingID objectID;
        
        bool shouldSaveConstructionObject = true;

        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Active, archive);
            auto foundObjectID = archive.objectTracker.FindID(object);
            if (foundObjectID.IsValid())
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

        SaveConstruction(
            object,
            archive,
            std::bool_constant<shouldConstructPolymorphically>{}
        );
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
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
            object = reinterpret_cast<ObjectT>(foundObject);
            return;
        }

        LoadConstruction(
            object,
            archive,
            objectID,
            std::bool_constant<shouldConstructPolymorphically>{}
        );
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::SaveConstruction(
        ObjectT& object, ArchiveT& archive, std::true_type)
    {
        TrackingID typeID;
        auto needsTypeHandle = false;
        auto type = std::type_index(typeid(*object));

        {
            auto foundTypeID = archive.typeTracker.FindID(type);
            needsTypeHandle = !foundTypeID.IsValid();
            if (foundTypeID.IsValid())
                typeID = *foundTypeID;
            else
                typeID = archive.typeTracker.Add(type);
        }

        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Inactive, archive);
            archive(typeID);
            if (needsTypeHandle)
            {
                auto typeHandle = archive.polymorphicManager.PrincipleTypeHandleFor(type);
                archive(typeHandle);
            }
        }

        archive.polymorphicManager.SaveConstruction(object, archive);
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::SaveConstruction(
        ObjectT& object, ArchiveT& archive, std::false_type)
    {
        bareScribe.Scriven(*object, archive);
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::LoadConstruction(
        ObjectT& object, ArchiveT& archive, TrackingID objectID, std::true_type)
    {
        TrackingID typeID;
        std::type_index type = typeid(void*);

        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Inactive, archive);
            archive(typeID);
            auto foundType = archive.typeTracker.FindType(typeID);
            if (!foundType.IsValid())
            {
                TypeHandle typeHandle;
                archive(typeHandle);

                type = archive.polymorphicManager.TypeIndexFor(typeHandle);
                archive.typeTracker.Add(type, typeID);
            }
            else
                type = *foundType;
        }

        auto storage = archive.polymorphicManager.CreateStorage(type);
        archive.objectTracker.Add(storage, objectID);
        archive.polymorphicManager.ConstructFromLoad(storage, type, archive);
        object = reinterpret_cast<ObjectT>(storage);
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::LoadConstruction(
        ObjectT& object, ArchiveT& archive, TrackingID objectID, std::false_type)
    {
        auto storage = reinterpret_cast<BareObject*>(CreateStorage(sizeof(BareObject)));
        Access::Construct(storage, archive, bareScribe);
        object = storage;
    }

    template<class Object, class Archive>
    class Scribe<Object*, Archive> : public PointerScribe<Object*, Archive>
    {};

    template<class Object>
    struct ObjectTrackingTraits<Object*, BinaryArchive>
    {
        static constexpr bool shouldTrack = false;
    };
}