#pragma once

#include <unordered_map>
#include <memory>

#include "ObjectTrackerEntry.h"
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

        void Activate(bool set = true);
        void Deactivate();
        bool IsActive() const;

        Optional<ID> Add(void* add);
        Optional<ID> Add(void* add, ID id);
        void ReplaceObject(void* here, void* newObject);

        void SignalSavedConstruction(ID id);
        bool HasSavedConstruction(ID id) const;

        void* FindObject(ID id);
        Optional<ID> FindID(void* object);

        void Clear();
    private:
        bool active = true;
    private:
        using Entry = ObjectTrackerEntry;
        using Map = std::unordered_map<ID, Entry>;
        using iterator = Map::iterator;
        Map map;

        Entry* FindEntry(ID id);
        iterator FindIterator(void* object);
    private:
        ID NextID() const;
    };
}