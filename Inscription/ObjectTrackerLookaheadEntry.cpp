#include "ObjectTrackerLookaheadEntry.h"

#include <utility>

#include "Storage.h"

namespace Inscription
{
    ObjectTrackerLookaheadEntry::ObjectTrackerLookaheadEntry(ID id, size_t storageSize) :
        ObjectTrackerEntryBase(id), storage(CreateStorage(storageSize)), storageSize(storageSize)
    {}

    ObjectTrackerLookaheadEntry::ObjectTrackerLookaheadEntry(const ObjectTrackerLookaheadEntry& arg) :
        ObjectTrackerEntryBase(arg), storage(CreateStorage(arg.storageSize)), storageSize(arg.storageSize)
    {}

    ObjectTrackerLookaheadEntry::ObjectTrackerLookaheadEntry(ObjectTrackerLookaheadEntry&& arg) :
        ObjectTrackerEntryBase(std::move(arg)), storage(std::move(arg.storage)), storageSize(std::move(storageSize))
    {}

    ObjectTrackerLookaheadEntry& ObjectTrackerLookaheadEntry::operator=(const ObjectTrackerLookaheadEntry& arg)
    {
        ObjectTrackerEntryBase::operator=(arg);
        storage = CreateStorage(arg.storageSize);
        storageSize = arg.storageSize;
        return *this;
    }

    ObjectTrackerLookaheadEntry& ObjectTrackerLookaheadEntry::operator=(ObjectTrackerLookaheadEntry&& arg)
    {
        ObjectTrackerEntryBase::operator=(std::move(arg));
        storage = std::move(arg.storage);
        storageSize = std::move(arg.storageSize);
        return *this;
    }

    ObjectTrackerLookaheadEntry::~ObjectTrackerLookaheadEntry()
    {
        DestroyStorage(storage);
    }

    ObjectTrackerLookaheadEntry* ObjectTrackerLookaheadEntry::Clone() const
    {
        return new ObjectTrackerLookaheadEntry(*this);
    }
}