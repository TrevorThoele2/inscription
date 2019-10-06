#pragma once

#include <unordered_map>
#include <optional>

#include "ObjectTrackerEntry.h"
#include "TrackingID.h"

namespace Inscription
{
    class ObjectTracker
    {
    public:
        using ID = TrackingID;
    public:
        ObjectTracker() = default;
        ObjectTracker(const ObjectTracker& arg);
        ObjectTracker(ObjectTracker&& arg) noexcept;

        ObjectTracker& operator=(const ObjectTracker& arg);
        ObjectTracker& operator=(ObjectTracker&& arg) noexcept;

        void Activate(bool set = true);
        void Deactivate();
        [[nodiscard]] bool IsActive() const;

        std::optional<ID> Add(void* add);
        std::optional<ID> Add(void* add, ID id);
        void ReplaceObject(void* here, void* newObject);

        void SignalSavedConstruction(ID id);
        [[nodiscard]] bool HasSavedConstruction(ID id) const;

        void* FindObject(ID id);
        std::optional<ID> FindID(void* object);

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
        [[nodiscard]] ID NextID() const;
    };
}