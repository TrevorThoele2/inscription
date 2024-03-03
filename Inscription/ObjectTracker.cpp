#include "ObjectTracker.h"

#include "Storage.h"

namespace Inscription
{
    ObjectTracker::ObjectTracker(const ObjectTracker& arg)
    {
        for (auto& loop : arg.map)
            map.emplace(loop.first, Entry(loop.second));
    }

    ObjectTracker::ObjectTracker(ObjectTracker&& arg) :
        active(arg.active), map(std::move(arg.map))
    {}

    ObjectTracker& ObjectTracker::operator=(const ObjectTracker& arg)
    {
        for (auto& loop : arg.map)
            map.emplace(loop.first, Entry(loop.second));

        return *this;
    }

    ObjectTracker& ObjectTracker::operator=(ObjectTracker&& arg)
    {
        active = arg.active;
        map = std::move(arg.map);
        return *this;
    }

    void ObjectTracker::Activate(bool set)
    {
        active = set;
    }

    void ObjectTracker::Deactivate()
    {
        active = false;
    }

    bool ObjectTracker::IsActive() const
    {
        return active;
    }

    Optional<ObjectTracker::ID> ObjectTracker::Add(void* add)
    {
        return Add(add, NextID());
    }

    Optional<ObjectTracker::ID> ObjectTracker::Add(void* add, ID id)
    {
        if (!IsActive())
            return Optional<ID>();

        {
            auto foundObject = FindObject(id);
            if (foundObject)
                return id;

            auto foundID = FindID(add);
            if (foundID.IsValid())
                return foundID;
        }

        Entry entry(id, add);
        map.emplace(id, std::move(entry));
        return id;
    }

    void ObjectTracker::ReplaceObject(void* here, void* newObject)
    {
        if (!IsActive())
            return;

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

    void ObjectTracker::SignalSavedConstruction(ID id)
    {
        if (!IsActive())
            return;

        auto iterator = map.find(id);
        if (iterator == map.end())
            return;

        iterator->second.hasSavedConstruction = true;
    }

    bool ObjectTracker::HasSavedConstruction(ID id) const
    {
        auto iterator = map.find(id);
        if (iterator == map.end())
            return false;

        return iterator->second.hasSavedConstruction;
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

    void ObjectTracker::Clear()
    {
        if (!IsActive())
            return;

        map.clear();
    }

    ObjectTracker::Entry* ObjectTracker::FindEntry(ID id)
    {
        auto entry = map.find(id);
        if (entry == map.end())
            return nullptr;

        return &entry->second;
    }

    ObjectTracker::iterator ObjectTracker::FindIterator(void* object)
    {
        for (auto loop = map.begin(); loop != map.end(); ++loop)
            if (loop->second.object == object)
                return loop;

        return map.end();
    }

    ObjectTracker::ID ObjectTracker::NextID() const
    {
        return map.size();
    }
}