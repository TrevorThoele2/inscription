#pragma once

#include <unordered_map>
#include <memory>

#include "ObjectTrackerEntry.h"
#include "ObjectTrackerLookaheadEntry.h"
#include "TrackingID.h"

#include "TypeMap.h"

#include "Optional.h"

namespace Inscription
{
    class ObjectTracker
    {
    public:
        using ID = TrackingID;
    public:
        ObjectTracker() = default;
        ObjectTracker(const ObjectTracker& arg);
        ObjectTracker(ObjectTracker&& arg);

        ObjectTracker& operator=(const ObjectTracker& arg);
        ObjectTracker& operator=(ObjectTracker&& arg);

        void SetActive(bool set = true);
        bool IsActive() const;

        Optional<ID> Add(void* add);
        Optional<ID> CreateLookahead(size_t memorySize);

        void* LookaheadMemory(ID id);
        void ActualizeLookahead(ID id);

        void* FindObject(ID id);
        Optional<ID> FindID(void* object);
        void ReplaceObject(void* here, void* newObject);

        void Clear();
    private:
        bool active = true;
    private:
        using Entry = ObjectTrackerEntry;
        using LookaheadEntry = ObjectTrackerLookaheadEntry;

        using EntryBase = ObjectTrackerEntryBase;
        using EntryBasePtr = std::unique_ptr<EntryBase>;
        using Map = std::unordered_map<ID, EntryBasePtr>;
        using iterator = Map::iterator;
        Map map;

        Optional<ID> Add(void* add, ID id);

        Entry* FindEntry(ID id);
        LookaheadEntry* FindLookaheadEntry(ID id);

        iterator FindIterator(void* object);
    private:
        ID NextID() const;
    };
}