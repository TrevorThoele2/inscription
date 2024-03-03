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

#include "Memory.h"

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
            ObjectT& object, ArchiveT& archive, TrackingID objectID, TrackingID typeID, std::true_type);
        static void SaveConstruction(
            ObjectT& object, ArchiveT& archive, TrackingID objectID, TrackingID typeID, std::false_type);
        static void LoadConstruction(
            ObjectT& object, ArchiveT& archive, TrackingID objectID, TrackingID typeID, std::true_type);
        static void LoadConstruction(
            ObjectT& object, ArchiveT& archive, TrackingID objectID, TrackingID typeID, std::false_type);
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

        auto& typeTracker = archive.typeTracker;
        auto& objectTracker = archive.objectTracker;

        TrackingID objectID;
        TrackingID typeID;
        bool shouldSaveConstruction;

        typeID = typeTracker.Add(typeid(*object));

        auto foundObjectID = objectTracker.FindID(object);
        if (foundObjectID.IsValid())
        {
            shouldSaveConstruction = false;
            objectID = *foundObjectID;
        }
        else
        {
            shouldSaveConstruction = true;
            objectID = *objectTracker.Add(object);
        }

        archive(objectID);
        archive(typeID);

        if (pointerType != Pointer::Owning || !shouldSaveConstruction)
            return;

        SaveConstruction(object, archive, objectID, typeID, std::bool_constant<shouldConstructPolymorphically>{});
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

        if (pointerType != Pointer::Owning)
        {
            archive.objectTracker.CreateLookahead(sizeof(BareObject));
            return;
        }

        LoadConstruction(object, archive, objectID, typeID, std::bool_constant<shouldConstructPolymorphically>{});
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::SaveConstruction(
        ObjectT& object, ArchiveT& archive, TrackingID objectID, TrackingID typeID, std::true_type)
    {
        auto& polymorphicManager = archive.polymorphicManager;

        auto className = polymorphicManager.ClassNameFor(typeID, archive);

        archive(className);
        polymorphicManager.SaveConstruction(object, archive);
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::SaveConstruction(
        ObjectT& object, ArchiveT& archive, TrackingID objectID, TrackingID typeID, std::false_type)
    {
        BareScribe::Scriven(*object, archive);
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::LoadConstruction(
        ObjectT& object, ArchiveT& archive, TrackingID objectID, TrackingID typeID, std::true_type)
    {
        ClassName className;
        archive(className);

        auto storage = archive.objectTracker.LookaheadMemory(objectID);

        bool wasLookahead = storage;
        archive.polymorphicManager.ConstructFromLoad(storage, className, archive);
        if (wasLookahead)
            archive.objectTracker.ActualizeLookahead(objectID);

        object = reinterpret_cast<Object>(storage);
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::LoadConstruction(
        ObjectT& object, ArchiveT& archive, TrackingID objectID, TrackingID typeID, std::false_type)
    {
        object = CreateStorage<BareObject>();
        BareScribe::Construct(object, archive);
    }

    template<class Object, class Archive>
    class Scribe<Object*, Archive> : public PointerScribe<Object*, Archive>
    {};
}