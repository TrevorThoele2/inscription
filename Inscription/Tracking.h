
#pragma once

#include <vector>
#include <type_traits>
#include <memory>
#include <unordered_set>

#include "TrackerID.h"
#include "TypeTable.h"
#include "Const.h"

#include "RegisteredTypeNotFoundException.h"

namespace Inscription
{
    class TrackingEntry
    {
    public:
        typedef TrackerID ID;
    public:
        const ID id;
        void* obj;
        bool hasBeenSerialized;
    public:
        TrackingEntry();
        TrackingEntry(ID id, void *obj);
        TrackingEntry(const TrackingEntry &arg);
    };

    class TrackingLookAheadEntry
    {
    public:
        void** position;
    public:
        TrackingLookAheadEntry(void** position);
    };

    class TrackingLookAheadGroup
    {
    public:
        typedef std::vector<TrackingLookAheadEntry> List;
    public:
        List list;
    public:
        TrackingLookAheadGroup() = default;
        TrackingLookAheadGroup(const TrackingLookAheadGroup& arg) = default;
        TrackingLookAheadGroup(TrackingLookAheadGroup&& arg);

        TrackingLookAheadGroup& operator=(const TrackingLookAheadGroup& arg) = default;
        TrackingLookAheadGroup& operator=(TrackingLookAheadGroup&& arg);
    };

    class Tracker
    {
    public:
        typedef TrackerID ID;
        typedef TrackingEntry Entry;
    public:
        Tracker() = default;
        Tracker(const Tracker &arg) = default;
        Tracker(Tracker &&arg);
        Tracker& operator=(const Tracker &arg) = default;
        Tracker& operator=(Tracker &&arg);

        ID Add(void *add, bool section);
        ID Add(void *add, bool section, ID id);
        void AddLookAhead(void** position);

        void ReplaceObject(void *here, void *newObj, bool section);
        Entry* Find(const void *find);
        const Entry* Find(const void *find) const;
        Entry* Find(ID id);
        const Entry* Find(ID id) const;
        void Clear();

        void ClearSection(bool deleteEntries);
    private:
        typedef std::unordered_map<ID, Entry> Map;
        typedef typename Map::iterator iterator;
        typedef typename Map::const_iterator const_iterator;

        typedef TrackingLookAheadGroup LookAheadGroup;
        typedef std::unordered_map<ID, LookAheadGroup> LookAheadMap;

        typedef std::unordered_map<const void*, iterator> Checker;
        typedef std::unordered_map<const void*, iterator> Section;
    private:
        Map map;
        Checker checker;

        LookAheadMap lookAheads;

        Section section;
    private:
        bool IsTracking(const void* obj) const;
    private:
        void AddLookAhead(void** position, ID id);
        LookAheadMap::iterator FindLookAheadList(ID id);
        void LinkLookAheads(void* toObj, ID id);
    private:
        ID NextID() const;
    };

    class TrackerGroup
    {
    public:
        typedef TrackerID ID;
        static constexpr ID nullID = 0;
    public:
        TrackerGroup();
        TrackerGroup(const TrackerGroup &arg) = default;
        TrackerGroup(TrackerGroup &&arg);
        TrackerGroup& operator=(const TrackerGroup &arg) = default;
        TrackerGroup& operator=(TrackerGroup &&arg);
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
        void ReplaceObject(T &here, T &newObj);

        void Clear();

        void StartSection();
        void StopSection();
        void ClearSection();
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
    private:
        friend class RegisteredTypes;
    };

    template<class T>
    TrackerGroup::ID TrackerGroup::Add(T *add)
    {
        if (!add)
            return nullID;

        if (!IsTypeInside<T>())
            return nullID;

        return Add(RemoveConst(add), typeid(*add));
    }

    template<class T>
    void TrackerGroup::AddLookAhead(T** add)
    {
        if (!add)
            return;

        // Find the tracker
        auto found = table.Find(typeid(T));
        if (!found)
            return;

        if (!IsActive())
            return;

        found->AddLookAhead(reinterpret_cast<void**>(add));
    }

    template<class T>
    void TrackerGroup::SignalSerialized(T* serialized)
    {
        auto found = FindEntry(serialized);
        if (!found)
            return;

        found->hasBeenSerialized = true;
    }

    template<class T>
    bool TrackerGroup::HasBeenSerialized(T* serialized)
    {
        auto found = FindEntry(serialized);
        if (!found)
            return false;

        return found->hasBeenSerialized;
    }

    template<class T>
    bool TrackerGroup::IsTypeInside() const
    {
        return table.Find(std::type_index(typeid(T))) != nullptr;
    }

    template<class T>
    T* TrackerGroup::FindObject(ID id)
    {
        if (id == nullID)
            return nullptr;

        auto found = FindEntry<T>(id);
        if (found)
            return static_cast<T*>(found->obj);
        else
            return nullptr;
    }

    template<class T>
    TrackerGroup::ID TrackerGroup::FindID(T* obj)
    {
        if (!obj)
            return nullID;

        auto found = FindEntry(obj);
        if (found)
            return found->id;
        else
            return nullID;
    }

    template<class T>
    void TrackerGroup::ReplaceObject(T &here, T &newObj)
    {
        if (typeid(here) != typeid(newObj))
            return;

        auto tableFound = table.Find(typeid(here));
        if (!tableFound)
            return;

        tableFound->ReplaceObject(&here, &newObj, usingSection);
    }

    template<class T>
    TrackingEntry* TrackerGroup::FindEntry(ID id)
    {
        auto tableFound = table.Find(std::type_index(typeid(T)));
        if (!tableFound)
            return nullptr;

        return tableFound->Find(id);
    }

    template<class T>
    TrackingEntry* TrackerGroup::FindEntry(T* obj)
    {
        auto tableFound = table.Find(std::type_index(typeid(T)));
        if (!tableFound)
            return nullptr;

        return tableFound->Find(obj);
    }

    class Scribe;
    class TrackingChangerStack
    {
    private:
        bool prev;
        Scribe &scribe;
    public:
        // ChangeTo is the value that the scribe's tracking will be changed into
        TrackingChangerStack(Scribe &scribe, bool changeTo);
        ~TrackingChangerStack();
    };
}