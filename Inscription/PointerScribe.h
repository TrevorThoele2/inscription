#pragma once

#include <string>

#include "ObjectTrackingContext.h"
#include "SpecialObjectTrackingID.h"

#include "NumericScribe.h"
#include "StringScribe.h"

namespace Inscription
{
    class BinaryArchive;
    class JsonArchive;

    template<class Object>
    class PointerScribeCategory final
    {
    public:
        using ObjectT = Object;
    public:
        static constexpr bool requiresScribe = false;
    public:
        static void Scriven(ObjectT*& object, BinaryArchive& archive);
        static void Scriven(const std::string& name, ObjectT*& object, JsonArchive& archive);
    private:
        template<class Archive>
        static void Save(ObjectT*& object, Archive& archive);
        template<class Archive>
        static void Load(ObjectT*& object, Archive& archive);

        template<class Archive>
        static void SaveConstruction(
            ObjectT*& object,
            Archive& archive,
            std::true_type);
        template<class Archive>
        static void SaveConstruction(
            ObjectT*& object,
            Archive& archive,
            std::false_type);
        template<class Archive>
        static void LoadConstruction(
            ObjectT*& object,
            Archive& archive,
            TrackingID objectID,
            std::true_type);
        template<class Archive>
        static void LoadConstruction(
            ObjectT*& object,
            Archive& archive,
            TrackingID objectID,
            std::false_type);
    };

    template<class Object>
    void PointerScribeCategory<Object>::Scriven(
        ObjectT*& object, BinaryArchive& archive)
    {
        if (archive.IsOutput())
            Save(object, archive);
        else
            Load(object, archive);
    }

    template<class Object>
    void PointerScribeCategory<Object>::Scriven(
        const std::string& name, ObjectT*& object, JsonArchive& archive)
    {
        if (archive.IsOutput())
            Save(object, archive);
        else
            Load(object, archive);
    }

    template<class Object>
    template<class Archive>
    void PointerScribeCategory<Object>::Save(ObjectT*& object, Archive& archive)
    {
        if (object == nullptr)
        {
            auto trackingContext = ObjectTrackingContext::Inactive(archive.types);
            archive(SpecialObjectTrackingID::NULLPTR);
            return;
        }

        TrackingID objectID;

        auto shouldSaveConstructionObject = true;

        {
            auto trackingContext = ObjectTrackingContext::Active(archive.types);
            auto foundObjectID = archive.types.FindObjectID(object);
            if (foundObjectID.has_value())
            {
                objectID = *foundObjectID;
                shouldSaveConstructionObject = false;
            }
            else
                objectID = *archive.types.AttemptTrackObject(object);
        }

        {
            auto trackingContext = ObjectTrackingContext::Inactive(archive.types);
            archive(objectID);
        }

        if (!shouldSaveConstructionObject || archive.types.HasSavedConstruction(objectID))
            return;

        archive.types.TrackSavedConstruction(objectID);

        SaveConstruction(
            object,
            archive,
            std::bool_constant<std::is_polymorphic_v<ObjectT>>{});
    }

    template<class Object>
    template<class Archive>
    void PointerScribeCategory<Object>::Load(ObjectT*& object, Archive& archive)
    {
        TrackingID objectID;

        {
            auto trackingContext = ObjectTrackingContext::Inactive(archive.types);
            archive(objectID);
        }

        if (objectID == SpecialObjectTrackingID::NULLPTR)
        {
            object = nullptr;
            return;
        }

        auto foundObject = archive.types.FindObject(objectID);
        if (foundObject)
        {
            object = reinterpret_cast<ObjectT*>(foundObject);
            return;
        }

        LoadConstruction(
            object,
            archive,
            objectID,
            std::bool_constant<std::is_polymorphic_v<ObjectT>>{});
    }

    template<class Object>
    template<class Archive>
    void PointerScribeCategory<Object>::SaveConstruction(ObjectT*& object, Archive& archive, std::true_type)
    {
        TrackingID typeID;
        bool needsType;
        auto typeIndex = std::type_index(typeid(*object));

        {
            auto foundTypeID = archive.types.FindTypeID(typeIndex);
            needsType = !foundTypeID.has_value();
            if (foundTypeID.has_value())
                typeID = *foundTypeID;
            else
                typeID = archive.types.AddType(typeIndex);
        }

        {
            auto trackingContext = ObjectTrackingContext::Inactive(archive.types);
            archive(typeID);
            if (needsType)
            {
                auto type = archive.types.PolymorphicOutputTypeFor(typeIndex);
                archive(type);
            }
        }

        archive.types.PolymorphicSave(object);
    }

    template<class Object>
    template<class Archive>
    void PointerScribeCategory<Object>::SaveConstruction(ObjectT*& object, Archive& archive, std::false_type)
    {
        ScrivenDispatch::Execute(*object, archive);
    }

    template<class Object>
    template<class Archive>
    void PointerScribeCategory<Object>::LoadConstruction(ObjectT*& object, Archive& archive, TrackingID objectID, std::true_type)
    {
        TrackingID typeID;
        std::type_index typeIndex = typeid(void*);

        {
            auto trackingContext = ObjectTrackingContext::Inactive(archive.types);
            archive(typeID);
            auto foundType = archive.types.FindType(typeID);
            if (!foundType.has_value())
            {
                Type type;
                archive(type);

                typeIndex = archive.types.PolymorphicTypeIndexFor(type);
                archive.types.AddType(typeIndex, typeID);
            }
            else
                typeIndex = *foundType;
        }

        auto storage = archive.types.CreatePolymorphicStorage(typeIndex);
        archive.types.AttemptTrackObject(storage, objectID);
        archive.types.ConstructPolymorphic(storage, typeIndex);
        object = reinterpret_cast<ObjectT*>(storage);
    }

    template<class Object>
    template<class Archive>
    void PointerScribeCategory<Object>::LoadConstruction(ObjectT*& object, Archive& archive, TrackingID objectID, std::false_type)
    {
        auto storage = reinterpret_cast<ObjectT*>(CreateStorage(sizeof(ObjectT)));
        ConstructDispatch::Execute(storage, archive);
        object = storage;
    }

    template<class Object, class Archive>
    struct ScribeTraits<Object*, Archive>
    {
        using Category = PointerScribeCategory<Object>;
    };
}