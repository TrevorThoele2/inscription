#pragma once

#include <memory>

#include "ScribeBase.h"

#include "ObjectTracker.h"
#include "TypeTracker.h"
#include "SpecialObjectTrackingID.h"
#include "SpecialTypeTrackingID.h"

#include "Pointer.h"
#include "PolymorphicManager.h"

#include "NumericScribe.h"
#include "StringScribe.h"

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
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
        static void Scriven(ObjectT& object, ArchiveT& archive, Pointer pointerType);
    public:
        using BaseT::Construct;
    private:
        using BareObject = std::remove_pointer_t<Object>;
    private:
        using BareScribe = Scribe<BareObject, ArchiveT>;
    private:
        static void SaveImplementation(ObjectT& object, ArchiveT& archive, Pointer pointerType);
        static void LoadImplementation(ObjectT& object, ArchiveT& archive, Pointer pointerType);
    private:
        constexpr static bool shouldConstructPolymorphically = std::is_polymorphic_v<BareObject>;

        static void SaveConstruction(
            ObjectT& object,
            ArchiveT& archive,
            TrackingID objectID,
            TrackingID typeID,
            Pointer pointerType,
            std::true_type);
        static void SaveConstruction(
            ObjectT& object,
            ArchiveT& archive,
            TrackingID objectID,
            TrackingID typeID,
            Pointer pointerType,
            std::false_type);
        static void LoadConstruction(
            ObjectT& object,
            ArchiveT& archive,
            TrackingID objectID,
            TrackingID typeID,
            Pointer pointerType,
            std::true_type);
        static void LoadConstruction(
            ObjectT& object,
            ArchiveT& archive,
            TrackingID objectID,
            TrackingID typeID,
            Pointer pointerType,
            std::false_type);
    private:
        PointerScribe() = delete;
        PointerScribe(const PointerScribe& arg) = delete;
        PointerScribe& operator=(const PointerScribe& arg) = delete;
    private:
        static_assert(std::is_pointer_v<ObjectT>,
            "The Object given to a PointerScribe was not a pointer.");
    };

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        Scriven(object, archive, Pointer::Unowning);
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::Scriven(ObjectT& object, ArchiveT& archive, Pointer pointerType)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive, pointerType);
        else
            LoadImplementation(object, archive, pointerType);
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::SaveImplementation(ObjectT& object, ArchiveT& archive, Pointer pointerType)
    {
        if (object == nullptr)
        {
            archive(SpecialObjectTrackingID::NULLPTR);
            archive(SpecialTypeTrackingID::NULLPTR);
            return;
        }

        TrackingID objectID;
        TrackingID typeID;
        
        typeID = archive.typeTracker.Add(typeid(*object));

        auto foundObjectID = archive.objectTracker.FindEntryID(object);
        if (foundObjectID.IsValid())
            objectID = *foundObjectID;
        else
            objectID = *archive.objectTracker.Add(object);

        archive(objectID);
        archive(typeID);

        SaveConstruction(
            object,
            archive,
            objectID,
            typeID, 
            pointerType,
            std::bool_constant<shouldConstructPolymorphically>{});
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::LoadImplementation(ObjectT& object, ArchiveT& archive, Pointer pointerType)
    {
        TrackingID objectID;
        TrackingID typeID;

        archive(objectID);
        archive(typeID);

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
            typeID,
            pointerType,
            std::bool_constant<shouldConstructPolymorphically>{});
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::SaveConstruction(
        ObjectT& object, ArchiveT& archive, TrackingID objectID, TrackingID typeID, Pointer pointerType, std::true_type)
    {
        auto className = archive.polymorphicManager.ClassNameFor(typeID, archive);
        archive.TrackObjects(false);
        archive(className);
        archive.TrackObjects(true);

        bool hasSavedConstruction = archive.objectTracker.HasSavedConstruction(objectID);
        if (pointerType != Pointer::Owning || hasSavedConstruction)
            return;

        archive.polymorphicManager.SaveConstruction(object, archive);

        archive.objectTracker.SignalSavedConstruction(objectID);
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::SaveConstruction(
        ObjectT& object, ArchiveT& archive, TrackingID objectID, TrackingID typeID, Pointer pointerType, std::false_type)
    {
        BareScribe::Scriven(*object, archive);
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::LoadConstruction(
        ObjectT& object, ArchiveT& archive, TrackingID objectID, TrackingID typeID, Pointer pointerType, std::true_type)
    {
        archive.TrackObjects(false);
        ClassName className;
        archive(className);
        archive.TrackObjects(true);

        if (pointerType != Pointer::Owning)
        {
            archive.polymorphicManager.CreateLookahead(className, objectID, archive);
            object = reinterpret_cast<ObjectT>(archive.objectTracker.LookaheadStorage(objectID));
            return;
        }

        auto storage = archive.objectTracker.LookaheadStorage(objectID);

        bool hadLookahead = storage;

        archive.polymorphicManager.ConstructFromLoad(storage, className, archive);

        if (hadLookahead)
            archive.objectTracker.MaterializeLookahead(objectID);

        object = reinterpret_cast<ObjectT>(storage);
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::LoadConstruction(
        ObjectT& object, ArchiveT& archive, TrackingID objectID, TrackingID typeID, Pointer pointerType, std::false_type)
    {
        if (pointerType != Pointer::Owning)
        {
            archive.objectTracker.CreateLookahead(objectID, sizeof(BareObject));
            object = reinterpret_cast<ObjectT>(archive.objectTracker.LookaheadStorage(objectID));
            return;
        }

        auto storage = archive.objectTracker.LookaheadStorage(objectID);

        bool hadLookahead = storage;
        if (hadLookahead)
            object = reinterpret_cast<ObjectT>(storage);
        else
            object = reinterpret_cast<ObjectT>(CreateStorage(sizeof(BareObject)));

        BareScribe::Construct(object, archive);

        if (hadLookahead)
            archive.objectTracker.MaterializeLookahead(objectID);
    }

    template<class Object, class Archive>
    class Scribe<Object*, Archive> : public PointerScribe<Object*, Archive>
    {};
}