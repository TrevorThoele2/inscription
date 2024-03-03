#pragma once

#include <string>

#include "ObjectTrackingContext.h"
#include "SpecialObjectTrackingID.h"

#include "NumericScribe.h"
#include "StringScribe.h"

namespace Inscription
{
    template<class Object>
    class PointerScribeCategory final
    {
    public:
        using ObjectT = Object;
    public:
        static constexpr bool requiresScribe = false;
    public:
        static void Scriven(ObjectT*& object, Format::Binary& format);
        static void Scriven(const std::string& name, ObjectT*& object, Format::Json& format);
    private:
        template<class Format>
        static void Save(ObjectT*& object, Format& format);
        template<class Format>
        static void Load(ObjectT*& object, Format& format);

        template<class Format>
        static void SaveConstruction(
            ObjectT*& object,
            Format& format,
            std::true_type);
        template<class Format>
        static void SaveConstruction(
            ObjectT*& object,
            Format& format,
            std::false_type);
        template<class Format>
        static void LoadConstruction(
            ObjectT*& object,
            Format& format,
            TrackingID objectID,
            std::true_type);
        template<class Format>
        static void LoadConstruction(
            ObjectT*& object,
            Format& format,
            TrackingID objectID,
            std::false_type);
    };

    template<class Object>
    void PointerScribeCategory<Object>::Scriven(
        ObjectT*& object, Format::Binary& format)
    {
        if (format.IsOutput())
            Save(object, format);
        else
            Load(object, format);
    }

    template<class Object>
    void PointerScribeCategory<Object>::Scriven(
        const std::string& name, ObjectT*& object, Format::Json& format)
    {
        if (format.IsOutput())
            Save(object, format);
        else
            Load(object, format);
    }

    template<class Object>
    template<class Format>
    void PointerScribeCategory<Object>::Save(ObjectT*& object, Format& format)
    {
        if (object == nullptr)
        {
            auto trackingContext = ObjectTrackingContext::Inactive(format.types);
            format(SpecialObjectTrackingID::NULLPTR);
            return;
        }

        TrackingID objectID;

        auto shouldSaveConstructionObject = true;

        {
            auto trackingContext = ObjectTrackingContext::Active(format.types);
            auto foundObjectID = format.types.FindObjectID(object);
            if (foundObjectID.has_value())
            {
                objectID = *foundObjectID;
                shouldSaveConstructionObject = false;
            }
            else
                objectID = *format.types.AttemptTrackObject(object);
        }

        {
            auto trackingContext = ObjectTrackingContext::Inactive(format.types);
            format(objectID);
        }

        if (!shouldSaveConstructionObject || format.types.HasSavedConstruction(objectID))
            return;

        format.types.TrackSavedConstruction(objectID);

        SaveConstruction(
            object,
            format,
            std::bool_constant<std::is_polymorphic_v<ObjectT>>{});
    }

    template<class Object>
    template<class Format>
    void PointerScribeCategory<Object>::Load(ObjectT*& object, Format& format)
    {
        TrackingID objectID;

        {
            auto trackingContext = ObjectTrackingContext::Inactive(format.types);
            format(objectID);
        }

        if (objectID == SpecialObjectTrackingID::NULLPTR)
        {
            object = nullptr;
            return;
        }

        auto foundObject = format.types.FindObject(objectID);
        if (foundObject)
        {
            object = reinterpret_cast<ObjectT*>(foundObject);
            return;
        }

        LoadConstruction(
            object,
            format,
            objectID,
            std::bool_constant<std::is_polymorphic_v<ObjectT>>{});
    }

    template<class Object>
    template<class Format>
    void PointerScribeCategory<Object>::SaveConstruction(ObjectT*& object, Format& format, std::true_type)
    {
        TrackingID typeID;
        bool needsType;
        auto typeIndex = std::type_index(typeid(*object));

        {
            auto foundTypeID = format.types.FindTypeID(typeIndex);
            needsType = !foundTypeID.has_value();
            if (foundTypeID.has_value())
                typeID = *foundTypeID;
            else
                typeID = format.types.AddType(typeIndex);
        }

        {
            auto trackingContext = ObjectTrackingContext::Inactive(format.types);
            format(typeID);
            if (needsType)
            {
                auto type = format.types.PolymorphicOutputTypeFor(typeIndex);
                format(type);
            }
        }

        format.types.PolymorphicSave(object);
    }

    template<class Object>
    template<class Format>
    void PointerScribeCategory<Object>::SaveConstruction(ObjectT*& object, Format& format, std::false_type)
    {
        ScrivenDispatch::Execute(*object, format);
    }

    template<class Object>
    template<class Format>
    void PointerScribeCategory<Object>::LoadConstruction(ObjectT*& object, Format& format, TrackingID objectID, std::true_type)
    {
        TrackingID typeID;
        std::type_index typeIndex = typeid(void*);

        {
            auto trackingContext = ObjectTrackingContext::Inactive(format.types);
            format(typeID);
            auto foundType = format.types.FindType(typeID);
            if (!foundType.has_value())
            {
                Type type;
                format(type);

                typeIndex = format.types.PolymorphicTypeIndexFor(type);
                format.types.AddType(typeIndex, typeID);
            }
            else
                typeIndex = *foundType;
        }

        auto storage = format.types.CreatePolymorphicStorage(typeIndex);
        format.types.AttemptTrackObject(storage, objectID);
        format.types.ConstructPolymorphic(storage, typeIndex);
        object = reinterpret_cast<ObjectT*>(storage);
    }

    template<class Object>
    template<class Format>
    void PointerScribeCategory<Object>::LoadConstruction(ObjectT*& object, Format& format, TrackingID objectID, std::false_type)
    {
        auto storage = reinterpret_cast<ObjectT*>(CreateStorage(sizeof(ObjectT)));
        ConstructDispatch::Execute(storage, format);
        object = storage;
    }

    template<class Object, class Format>
    struct ScribeTraits<Object*, Format>
    {
        using Category = PointerScribeCategory<Object>;
    };
}