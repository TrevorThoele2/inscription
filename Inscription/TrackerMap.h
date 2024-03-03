#pragma once

#include <typeindex>

#include "Tracker.h"
#include "TrackingID.h"
#include "TrackingEntry.h"

#include "TypeTable.h"

#include "Const.h"

namespace Inscription
{
    class TrackerMap
    {
    public:
        typedef TrackingID ID;
    public:
        TrackerMap();
        TrackerMap(const TrackerMap& arg) = default;
        TrackerMap(TrackerMap&& arg);

        TrackerMap& operator=(const TrackerMap& arg) = default;
        TrackerMap& operator=(TrackerMap&& arg);

        void SetActive(bool set = true);
        bool IsActive() const;

        template<class T>
        ID Add(T* add);
        ID Add(void* add, const std::type_index& type);
        template<class T>
        void AddLookAhead(T** add);

        template<class T>
        void SignalSerialized(T* serialized);
        template<class T>
        bool HasBeenSerialized(T* serialized);

        template<class T>
        bool IsTypeInside() const;
        template<class T>
        T* FindObject(ID id);
        template<class T>
        ID FindID(T* obj);
        template<class T>
        void ReplaceObject(T& here, T& newObj);

        void Clear();

        void StartSection();
        void StopSection();
        void ClearSection();

        template<class T>
        void Register();
    private:
        typedef TypeTable<Tracker> Table;
    private:
        bool active;
        Table table;

        bool usingSection;
    private:
        template<class T>
        TrackingEntry* FindEntry(ID id);
        template<class T>
        TrackingEntry* FindEntry(T* obj);
    };

    template<class T>
    TrackerMap::ID TrackerMap::Add(T* add)
    {
        if (!add)
            return invalidTrackingID;

        if (!IsTypeInside<T>())
            return invalidTrackingID;

        return Add(RemoveConst(add), typeid(*add));
    }

    template<class T>
    void TrackerMap::AddLookAhead(T** add)
    {
        if (!add)
            return;

        // Find the tracker
        auto found = table.Find(typeid(T));
        if (!found)
            return;

        if (!IsActive())
            return;

        found->AddLookahead(reinterpret_cast<void**>(add));
    }

    template<class T>
    void TrackerMap::SignalSerialized(T* serialized)
    {
        auto found = FindEntry(serialized);
        if (!found)
            return;

        found->hasBeenSerialized = true;
    }

    template<class T>
    bool TrackerMap::HasBeenSerialized(T* serialized)
    {
        auto found = FindEntry(serialized);
        if (!found)
            return false;

        return found->hasBeenSerialized;
    }

    template<class T>
    bool TrackerMap::IsTypeInside() const
    {
        return table.Find(std::type_index(typeid(T))) != nullptr;
    }

    template<class T>
    T* TrackerMap::FindObject(ID id)
    {
        if (id == invalidTrackingID)
            return nullptr;

        auto found = FindEntry<T>(id);
        if (found)
            return static_cast<T*>(found->obj);
        else
            return nullptr;
    }

    template<class T>
    TrackerMap::ID TrackerMap::FindID(T* obj)
    {
        if (!obj)
            return invalidTrackingID;

        auto found = FindEntry(obj);
        if (found)
            return found->id;
        else
            return invalidTrackingID;
    }

    template<class T>
    void TrackerMap::ReplaceObject(T& here, T& newObj)
    {
        if (typeid(here) != typeid(newObj))
            return;

        auto tableFound = table.Find(typeid(here));
        if (!tableFound)
            return;

        tableFound->ReplaceObject(&here, &newObj, usingSection);
    }

    template<class T>
    TrackingEntry* TrackerMap::FindEntry(ID id)
    {
        auto tableFound = table.Find(std::type_index(typeid(T)));
        if (!tableFound)
            return nullptr;

        return tableFound->Find(id);
    }

    template<class T>
    TrackingEntry* TrackerMap::FindEntry(T* obj)
    {
        auto tableFound = table.Find(std::type_index(typeid(T)));
        if (!tableFound)
            return nullptr;

        return tableFound->Find(obj);
    }

    template<class T>
    void TrackerMap::Register()
    {
        table.Add(Tracker(), typeid(T));
    }
}