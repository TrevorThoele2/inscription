#pragma once

#include <typeindex>

#include "ObjectTrackingMap.h"
#include "ObjectTrackingMapEntry.h"
#include "TrackingID.h"

#include "TypeMap.h"

#include "Optional.h"

#include "RegisteredTypeNotFound.h"

namespace Inscription
{
    class ObjectTracker
    {
    public:
        typedef TrackingID ID;
    public:
        ObjectTracker();
        ObjectTracker(const ObjectTracker& arg) = default;
        ObjectTracker(ObjectTracker&& arg);

        ObjectTracker& operator=(const ObjectTracker& arg) = default;
        ObjectTracker& operator=(ObjectTracker&& arg);

        void SetActive(bool set = true);
        bool IsActive() const;

        template<class T>
        Optional<ID> Add(T* add);
        template<class T>
        Optional<ID> AttemptAdd(T* add);
        Optional<ID> Add(void* add, const std::type_index& type);

        template<class T>
        void SignalSerialized(T* serialized);
        template<class T>
        bool HasBeenSerialized(T* serialized);

        template<class T>
        bool IsTypeInside() const;
        template<class T>
        T* FindObject(ID id);
        template<class T>
        Optional<ID> FindID(T* obj);
        template<class T>
        void ReplaceObject(T& here, T& newObj);

        void Clear();

        void StartSection();
        void StopSection();
        void ClearSection();

        template<class T>
        void Register();
    private:
        typedef TypeMap<ObjectTrackingMap> TypeMap;
    private:
        bool active;
        TypeMap typeMap;

        bool usingSection;
    private:
        template<class T>
        ObjectTrackingMapEntry* FindEntry(ID id);
        template<class T>
        ObjectTrackingMapEntry* FindEntry(T* obj);
    };

    template<class T>
    Optional<ObjectTracker::ID> ObjectTracker::Add(T* add)
    {
        if (!add)
            return Optional<ID>();

        if (!IsTypeInside<T>())
            throw RegisteredTypeNotFound(typeid(T));

        return Add(RemoveConst(add), typeid(*add));
    }

    template<class T>
    Optional<ObjectTracker::ID> ObjectTracker::AttemptAdd(T* add)
    {
        if (!add)
            return Optional<ID>();

        if (!IsTypeInside<T>())
            return Optional<ID>();

        return Add(RemoveConst(add), typeid(*add));
    }

    template<class T>
    void ObjectTracker::SignalSerialized(T* serialized)
    {
        auto found = FindEntry(serialized);
        if (!found)
            return;

        found->hasBeenSerialized = true;
    }

    template<class T>
    bool ObjectTracker::HasBeenSerialized(T* serialized)
    {
        auto found = FindEntry(serialized);
        if (!found)
            return false;

        return found->hasBeenSerialized;
    }

    template<class T>
    bool ObjectTracker::IsTypeInside() const
    {
        return typeMap.Find(std::type_index(typeid(T))) != nullptr;
    }

    template<class T>
    T* ObjectTracker::FindObject(ID id)
    {
        if (id == SpecialObjectTrackingID::NULLPTR)
            return nullptr;

        auto found = FindEntry<T>(id);
        if (found)
            return static_cast<T*>(found->obj);
        else
            return nullptr;
    }

    template<class T>
    Optional<ObjectTracker::ID> ObjectTracker::FindID(T* obj)
    {
        if (!obj)
            return Optional<ID>();

        auto found = FindEntry(obj);
        if (found)
            return found->id;
        else
            return Optional<ID>();
    }

    template<class T>
    void ObjectTracker::ReplaceObject(T& here, T& newObj)
    {
        if (typeid(here) != typeid(newObj))
            return;

        auto found = typeMap.Find(typeid(here));
        if (!found)
            return;

        found->ReplaceObject(&here, &newObj, usingSection);
    }

    template<class T>
    ObjectTrackingMapEntry* ObjectTracker::FindEntry(ID id)
    {
        auto found = typeMap.Find(std::type_index(typeid(T)));
        if (!found)
            return nullptr;

        return found->Find(id);
    }

    template<class T>
    ObjectTrackingMapEntry* ObjectTracker::FindEntry(T* obj)
    {
        auto found = typeMap.Find(std::type_index(typeid(T)));
        if (!found)
            return nullptr;

        return found->Find(obj);
    }

    template<class T>
    void ObjectTracker::Register()
    {
        typeMap.Add(ObjectTrackingMap(), typeid(T));
    }
}