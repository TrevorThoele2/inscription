#pragma once

#include <memory>

#include "ScribeBase.h"

#include "ObjectTracker.h"
#include "TypeTracker.h"
#include "SpecialObjectTrackingID.h"
#include "SpecialTypeTrackingID.h"

#include "PolymorphicManager.h"

#include "NumericScribe.h"
#include "String.h"

#include "Optional.h"

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
    public:
        using BaseT::Construct;
    private:
        using BareObject = std::remove_pointer_t<Object>;
    private:
        using BareScribe = Scribe<BareObject, ArchiveT>;
    private:
        static void SaveImplementation(ObjectT& object, ArchiveT& archive);
        static void LoadImplementation(ObjectT& object, ArchiveT& archive);
    private:
        constexpr static bool shouldConstructPolymorphically = std::is_polymorphic_v<BareObject>;

        static void SaveConstruction(ObjectT& object, ArchiveT& archive, TrackingID typeID, std::true_type);
        static void SaveConstruction(ObjectT& object, ArchiveT& archive, TrackingID typeID, std::false_type);
        static void LoadConstruction(ObjectT& object, ArchiveT& archive, TrackingID typeID, std::true_type);
        static void LoadConstruction(ObjectT& object, ArchiveT& archive, TrackingID typeID, std::false_type);
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

        if (!shouldSaveConstruction)
            return;

        SaveConstruction(object, archive, typeID, std::bool_constant<shouldConstructPolymorphically>{});
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
    {
        TrackingID objectID;
        TrackingID typeID;

        archive(objectID);
        archive(typeID);

        auto foundObject = archive.objectTracker.FindObject<Object>(objectID);
        if (foundObject)
        {
            object = *foundObject;
            return;
        }

        LoadConstruction(object, archive, typeID, std::bool_constant<shouldConstructPolymorphically>{});
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::SaveConstruction(ObjectT& object, ArchiveT& archive, TrackingID typeID, std::true_type)
    {
        auto& polymorphicManager = archive.polymorphicManager;

        auto className = polymorphicManager.ClassNameFor(typeID, archive);

        archive(className);
        polymorphicManager.SaveConstruction(object, archive);
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::SaveConstruction(ObjectT& object, ArchiveT& archive, TrackingID typeID, std::false_type)
    {
        BareScribe::Scriven(*object, archive);
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::LoadConstruction(ObjectT& object, ArchiveT& archive, TrackingID typeID, std::true_type)
    {
        ClassName className;
        archive(className);

        void* constructed = nullptr;
        archive.polymorphicManager.ConstructFromLoad(constructed, className, archive);
        object = reinterpret_cast<ObjectT>(constructed);
    }

    template<class Object, class Archive>
    void PointerScribe<Object, Archive>::LoadConstruction(ObjectT& object, ArchiveT& archive, TrackingID typeID, std::false_type)
    {
        BareScribe::Construct(object, archive);
    }

    template<class Object, class Archive>
    class Scribe<Object*, Archive> : public PointerScribe<Object*, Archive>
    {};
}