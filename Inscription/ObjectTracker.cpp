#include "ObjectTracker.h"

#include "Storage.h"

namespace Inscription
{
    ObjectTracker::ObjectTracker(const ObjectTracker& arg)
    {
        for (auto& loop : arg.map)
            map.emplace(loop.first, EntryBasePtr(loop.second->Clone()));
    }

    ObjectTracker::ObjectTracker(ObjectTracker&& arg) :
        active(arg.active), map(std::move(arg.map))
    {}

    ObjectTracker& ObjectTracker::operator=(const ObjectTracker& arg)
    {
        for (auto& loop : arg.map)
            map.emplace(loop.first, EntryBasePtr(loop.second->Clone()));

        return *this;
    }

    ObjectTracker& ObjectTracker::operator=(ObjectTracker&& arg)
    {
        active = arg.active;
        map = std::move(arg.map);
        return *this;
    }

    void ObjectTracker::SetActive(bool set)
    {
        active = set;
    }

    bool ObjectTracker::IsActive() const
    {
        return active;
    }

    Optional<ObjectTracker::ID> ObjectTracker::Add(void* add)
    {
        if (!IsActive())
            return Optional<ID>();

        return Add(add, NextID());
    }

    Optional<ObjectTracker::ID> ObjectTracker::CreateLookahead(size_t storageSize)
    {
        return CreateLookahead(NextID(), storageSize);
    }

    Optional<ObjectTracker::ID> ObjectTracker::CreateLookahead(ID id, size_t storageSize)
    {
        if (!IsActive())
            return Optional<ID>();

        auto entry = EntryBasePtr(new LookaheadEntry(id, storageSize));

        map.emplace(id, std::move(entry));

        return id;
    }

    void* ObjectTracker::LookaheadStorage(ID id)
    {
        if (!IsActive())
            return nullptr;

        auto entry = FindLookaheadEntry(id);
        if (!entry)
            return nullptr;

        return entry->storage;
    }

    void ObjectTracker::MaterializeLookahead(ID id)
    {
        if (!IsActive())
            return;

        auto iterator = map.find(id);
        if (iterator == map.end())
            return;

        auto entry = dynamic_cast<LookaheadEntry*>(iterator->second.get());
        if (!entry)
            return;

        auto storage = entry->storage;
        entry->storage = nullptr;
        map.erase(iterator);
        Add(storage, id);
    }

    void ObjectTracker::SignalSavedConstruction(ID id)
    {
        if (!IsActive())
            return;

        auto iterator = map.find(id);
        if (iterator == map.end())
            return;

        iterator->second->hasSavedConstruction = true;
    }

    bool ObjectTracker::HasSavedConstruction(ID id) const
    {
        if (!IsActive())
            return false;

        auto iterator = map.find(id);
        if (iterator == map.end())
            return false;

        return iterator->second->hasSavedConstruction;
    }

    void* ObjectTracker::FindObject(ID id)
    {
        auto entry = FindEntry(id);
        if (!entry)
            return nullptr;

        return entry->object;
    }

    Optional<ObjectTracker::ID> ObjectTracker::FindID(void* object)
    {
        auto iterator = FindIterator(object);
        if (iterator == map.end())
            return Optional<ID>();

        return Optional<ID>(iterator->first);
    }

    Optional<ObjectTracker::ID> ObjectTracker::FindEntryID(void* object)
    {
        auto iterator = FindIterator(object);
        if (iterator == map.end())
            return Optional<ID>();
        
        auto casted = dynamic_cast<Entry*>(iterator->second.get());
        if (!casted)
            return Optional<ID>();

        return Optional<ID>(iterator->first);
    }

    void ObjectTracker::ReplaceObject(void* here, void* newObject)
    {
        auto iterator = FindIterator(here);
        if (iterator == map.end())
        {
            Add(newObject);
            return;
        }

        auto id = iterator->first;
        map.erase(iterator);
        Add(newObject, id);
    }

    void ObjectTracker::Clear()
    {
        map.clear();
    }

    Optional<ObjectTracker::ID> ObjectTracker::Add(void* add, ID id)
    {
        auto entry = EntryBasePtr(new Entry(id, add));
        map.emplace(id, std::move(entry));
        return id;
    }

    ObjectTracker::Entry* ObjectTracker::FindEntry(ID id)
    {
        auto entry = map.find(id);
        if (entry == map.end())
            return nullptr;

        return dynamic_cast<Entry*>(entry->second.get());
    }

    ObjectTracker::LookaheadEntry* ObjectTracker::FindLookaheadEntry(ID id)
    {
        auto entry = map.find(id);
        if (entry == map.end())
            return nullptr;

        return dynamic_cast<LookaheadEntry*>(entry->second.get());
    }

    ObjectTracker::iterator ObjectTracker::FindIterator(void* object)
    {
        for (auto loop = map.begin(); loop != map.end(); ++loop)
        {
            auto casted = dynamic_cast<Entry*>(loop->second.get());
            if (!casted)
                continue;

            if (casted->object == object)
                return loop;
        }

        return map.end();
    }

    ObjectTracker::ID ObjectTracker::NextID() const
    {
        return map.size();
    }
}