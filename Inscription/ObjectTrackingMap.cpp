#include "ObjectTrackingMap.h"

namespace Inscription
{
    ObjectTrackingMap::ObjectTrackingMap(ObjectTrackingMap&& arg) :
        map(std::move(arg.map)), checker(std::move(arg.checker)), section(std::move(arg.section))
    {}

    ObjectTrackingMap& ObjectTrackingMap::operator=(ObjectTrackingMap&& arg)
    {
        map = std::move(arg.map);
        checker = std::move(arg.checker);
        section = std::move(arg.section);
        return *this;
    }

    ObjectTrackingMap::ID ObjectTrackingMap::Add(void* add, bool section)
    {
        if (IsTracking(add))
            return Find(add)->id;

        return Add(add, section, NextID());
    }

    ObjectTrackingMap::ID ObjectTrackingMap::Add(void* add, bool section, ID id)
    {
        auto emplaced = map.emplace(id, Entry(id, add));
        checker.emplace(add, emplaced.first);
        if (section)
            this->section.emplace(add, emplaced.first);

        return id;
    }

    void ObjectTrackingMap::ReplaceObject(void* here, void* newObj, bool section)
    {
        auto found = checker.find(here);
        if (found != checker.end())
        {
            auto id = found->second->first;
            map.erase(found->second);
            checker.erase(here);
            this->section.erase(here);
            Add(newObj, section, id);
        }
    }

    ObjectTrackingMap::Entry* ObjectTrackingMap::Find(const void* find)
    {
        auto checkerFound = checker.find(find);
        if (checkerFound == checker.end())
            return nullptr;
        else
            return &checkerFound->second->second;
    }

    const ObjectTrackingMap::Entry* ObjectTrackingMap::Find(const void* find) const
    {
        return const_cast<ObjectTrackingMap*>(this)->Find(find);
    }

    ObjectTrackingMap::Entry* ObjectTrackingMap::Find(ID id)
    {
        auto found = map.find(id);
        if (found == map.end())
            return nullptr;
        else
            return &found->second;
    }

    const ObjectTrackingMap::Entry* ObjectTrackingMap::Find(ID id) const
    {
        return const_cast<ObjectTrackingMap*>(this)->Find(id);
    }

    void ObjectTrackingMap::Clear()
    {
        map.clear();
        checker.clear();
        section.clear();
    }

    void ObjectTrackingMap::ClearSection(bool deleteEntries)
    {
        if (deleteEntries)
        {
            for (auto &loop : section)
            {
                checker.erase(loop.second->second.obj);
                map.erase(loop.second);
            }
        }

        section.clear();
    }

    bool ObjectTrackingMap::IsTracking(const void* obj) const
    {
        return Find(obj) != nullptr;
    }

    ObjectTrackingMap::ID ObjectTrackingMap::NextID() const
    {
        return map.size();
    }
}