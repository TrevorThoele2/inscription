#pragma once

#include "ObjectTrackerEntryBase.h"

namespace Inscription
{
    class ObjectTrackerLookaheadEntry : public ObjectTrackerEntryBase
    {
    public:
        std::uint8_t* storage = nullptr;
        size_t storageSize = 0;
    public:
        ObjectTrackerLookaheadEntry() = default;
        ObjectTrackerLookaheadEntry(ID id, size_t storageSize);
        ObjectTrackerLookaheadEntry(const ObjectTrackerLookaheadEntry& arg);
        ObjectTrackerLookaheadEntry(ObjectTrackerLookaheadEntry&& arg);

        ObjectTrackerLookaheadEntry& operator=(const ObjectTrackerLookaheadEntry& arg);
        ObjectTrackerLookaheadEntry& operator=(ObjectTrackerLookaheadEntry&& arg);

        ~ObjectTrackerLookaheadEntry();

        ObjectTrackerLookaheadEntry* Clone() const override;
    };
}