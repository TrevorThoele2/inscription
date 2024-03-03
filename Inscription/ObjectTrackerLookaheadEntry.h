#pragma once

#include "ObjectTrackerEntryBase.h"

namespace Inscription
{
    class ObjectTrackerLookaheadEntry : public ObjectTrackerEntryBase
    {
    public:
        void* memory = nullptr;
        size_t memorySize = 0;
    public:
        ObjectTrackerLookaheadEntry() = default;
        ObjectTrackerLookaheadEntry(ID id, void* memory, size_t memorySize);
        ObjectTrackerLookaheadEntry(const ObjectTrackerLookaheadEntry& arg);

        ObjectTrackerLookaheadEntry* Clone() const override;
    };
}