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
    class ObjectScribe;

    template<class Object>
    class ObjectScribe<Object, BinaryArchive> : public TrackingScribe<Object, BinaryArchive>
    {
    private:
        using BaseT = TrackingScribe<Object, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        using BaseT::Scriven;
        void Scriven(ObjectT*& object, ArchiveT& archive);
    protected:
        ObjectScribe() = default;
        ObjectScribe(const ObjectScribe& arg) = default;

        using BaseT::ScrivenImplementation;
    private:
        void Save(ObjectT*& object, ArchiveT& archive);
        void Load(ObjectT*& object, ArchiveT& archive);
    private:
        void SaveConstruction(
            ObjectT*& object,
            ArchiveT& archive,
            std::true_type);
        void SaveConstruction(
            ObjectT*& object,
            ArchiveT& archive,
            std::false_type);
        void LoadConstruction(
            ObjectT*& object,
            ArchiveT& archive,
            TrackingID objectID,
            std::true_type);
        void LoadConstruction(
            ObjectT*& object,
            ArchiveT& archive,
            TrackingID objectID,
            std::false_type);
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
            "The Object given to a ObjectScribe was not an object.");
    };

    template<class Object>
    void ObjectScribe<Object, BinaryArchive>::Scriven(ObjectT*& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            Save(object, archive);
        else
            Load(object, archive);
    }

    template<class Object>
    void ObjectScribe<Object, BinaryArchive>::Save(
        ObjectT*& object, ArchiveT& archive)
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
    void ObjectScribe<Object, BinaryArchive>::Load(
        ObjectT*& object, ArchiveT& archive)
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
    void ObjectScribe<Object, BinaryArchive>::SaveConstruction(
        ObjectT*& object, ArchiveT& archive, std::true_type)
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
    void ObjectScribe<Object, BinaryArchive>::SaveConstruction(
        ObjectT*& object, ArchiveT& archive, std::false_type)
    {
        Scriven(*object, archive);
    }

    template<class Object>
    void ObjectScribe<Object, BinaryArchive>::LoadConstruction(
        ObjectT*& object, ArchiveT& archive, TrackingID objectID, std::true_type)
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
    void ObjectScribe<Object, BinaryArchive>::LoadConstruction(
        ObjectT*& object, ArchiveT& archive, TrackingID objectID, std::false_type)
    {
        auto storage = reinterpret_cast<ObjectT*>(CreateStorage(sizeof(ObjectT)));
        Access::Construct(storage, archive, AsScribe());
        object = storage;
    }

    template<class Object>
    class ObjectScribe<Object, JsonArchive> : public TrackingScribe<Object, JsonArchive>
    {
    private:
        using BaseT = TrackingScribe<Object, JsonArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        using BaseT::Scriven;
        void Scriven(const std::string& name, ObjectT*& object, ArchiveT& archive);
    protected:
        ObjectScribe() = default;
        ObjectScribe(const ObjectScribe& arg) = default;

        using BaseT::ScrivenImplementation;
    private:
        void Save(const std::string& name, ObjectT*& object, ArchiveT& archive);
        void Load(const std::string& name, ObjectT*& object, ArchiveT& archive);
    private:
        void SaveConstruction(
            const std::string& name,
            ObjectT*& object,
            ArchiveT& archive,
            std::true_type);
        void SaveConstruction(
            const std::string& name,
            ObjectT*& object,
            ArchiveT& archive,
            std::false_type);
        void LoadConstruction(
            const std::string& name,
            ObjectT*& object,
            ArchiveT& archive,
            TrackingID objectID,
            std::true_type);
        void LoadConstruction(
            const std::string& name,
            ObjectT*& object,
            ArchiveT& archive,
            TrackingID objectID,
            std::false_type);
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
            "The Object given to a ObjectScribe was not an object.");
    };

    template<class Object>
    void ObjectScribe<Object, JsonArchive>::Scriven(const std::string& name, ObjectT*& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            Save(object, archive);
        else
            Load(object, archive);
    }

    template<class Object>
    void ObjectScribe<Object, JsonArchive>::Save(
        const std::string& name, ObjectT*& object, ArchiveT& archive)
    {
        if (object == nullptr)
        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Inactive, archive.types);
            archive(name, SpecialObjectTrackingJsonID::NULLPTR);
            return;
        }

        TrackingID objectID;

        auto shouldSaveConstructionObject = true;

        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Active, archive.types);
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
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Inactive, archive.types);
            archive(name, objectID);
        }

        if (!shouldSaveConstructionObject || archive.types.HasSavedConstruction(objectID))
            return;

        archive.types.TrackSavedConstruction(objectID);

        SaveConstruction(
            name,
            object,
            archive,
            std::bool_constant<std::is_polymorphic_v<ObjectT>>{});
    }

    template<class Object>
    void ObjectScribe<Object, JsonArchive>::Load(
        const std::string& name, ObjectT*& object, ArchiveT& archive)
    {
        std::string value;

        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Inactive, archive.types);
            archive(name, value);
        }

        if (value == SpecialObjectTrackingJsonID::NULLPTR)
        {
            object = nullptr;
            return;
        }

        const auto objectID = Chroma::FromString<TrackingID>(value);

        auto foundObject = archive.types.FindObject(objectID);
        if (foundObject)
        {
            object = reinterpret_cast<ObjectT*>(foundObject);
            return;
        }

        LoadConstruction(
            name,
            object,
            archive,
            objectID,
            std::bool_constant<std::is_polymorphic_v<ObjectT>>{});
    }

    template<class Object>
    void ObjectScribe<Object, JsonArchive>::SaveConstruction(
        const std::string& name, ObjectT*& object, ArchiveT& archive, std::true_type)
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
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Inactive, archive.types);
            archive(name + "Construction", typeID);
            if (needsType)
            {
                auto type = archive.types.PolymorphicOutputTypeFor(typeIndex);
                archive(type);
            }
        }

        archive.types.PolymorphicSave(object);
    }

    template<class Object>
    void ObjectScribe<Object, JsonArchive>::SaveConstruction(
        const std::string& name, ObjectT*& object, ArchiveT& archive, std::false_type)
    {
        Scriven(name, *object, archive);
    }

    template<class Object>
    void ObjectScribe<Object, JsonArchive>::LoadConstruction(
        const std::string& name, ObjectT*& object, ArchiveT& archive, TrackingID objectID, std::true_type)
    {
        std::type_index typeIndex = typeid(void*);

        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Inactive, archive.types);
            std::string value;
            archive(name + "Construction", value);
            auto typeID = Chroma::FromString<TrackingID>(value);
            auto foundType = archive.types.FindType(typeID);
            if (!foundType.has_value())
            {
                Type type;
                archive(name + "Type", type);

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
    void ObjectScribe<Object, JsonArchive>::LoadConstruction(
        const std::string& name, ObjectT*& object, ArchiveT& archive, TrackingID objectID, std::false_type)
    {
        auto storage = reinterpret_cast<ObjectT*>(CreateStorage(sizeof(ObjectT)));
        Access::Construct(storage, archive, AsScribe());
        object = storage;
    }
}