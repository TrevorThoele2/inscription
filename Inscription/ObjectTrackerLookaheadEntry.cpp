#include "ObjectTrackerLookaheadEntry.h"

namespace Inscription
{
    ObjectTrackerLookaheadEntry::ObjectTrackerLookaheadEntry(ID id, void* memory, size_t memorySize) :
        ObjectTrackerEntryBase(id), memory(memory), memorySize(memorySize)
    {}

    ObjectTrackerLookaheadEntry::ObjectTrackerLookaheadEntry(const ObjectTrackerLookaheadEntry& arg) :
        ObjectTrackerEntryBase(arg), memory(arg.memory), memorySize(arg.memorySize)
    {}

    ObjectTrackerLookaheadEntry* ObjectTrackerLookaheadEntry::Clone() const
    {
        return new ObjectTrackerLookaheadEntry(*this);
    }
}