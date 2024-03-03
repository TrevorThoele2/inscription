#include "Tracker.h"

namespace Inscription
{
    Tracker::Tracker(Tracker&& arg) :
        map(std::move(arg.map)), checker(std::move(arg.checker)), section(std::move(arg.section))
    {}

    Tracker& Tracker::operator=(Tracker&& arg)
    {
        map = std::move(arg.map);
        checker = std::move(arg.checker);
        section = std::move(arg.section);
        return *this;
    }

    Tracker::ID Tracker::Add(void* add, bool section)
    {
        if (IsTracking(add))
        {
            auto id = Find(add)->id;
            LinkLookaheads(add, id);
            return id;
        }

        return Add(add, section, NextID());
    }

    Tracker::ID Tracker::Add(void* add, bool section, ID id)
    {
        auto emplaced = map.emplace(id, Entry(id, add));
        checker.emplace(add, emplaced.first);
        if (section)
            this->section.emplace(add, emplaced.first);

        LinkLookaheads(add, id);
        return id;
    }

    void Tracker::AddLookahead(void** position)
    {
        if (IsTracking(*position))
            return;

        AddLookahead(position, NextID());
    }

    void Tracker::ReplaceObject(void* here, void* newObj, bool section)
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

    Tracker::Entry* Tracker::Find(const void* find)
    {
        auto checkerFound = checker.find(find);
        if (checkerFound == checker.end())
            return nullptr;
        else
            return &checkerFound->second->second;
    }

    const Tracker::Entry* Tracker::Find(const void* find) const
    {
        return const_cast<Tracker*>(this)->Find(find);
    }

    Tracker::Entry* Tracker::Find(ID id)
    {
        auto found = map.find(id);
        if (found == map.end())
            return nullptr;
        else
            return &found->second;
    }

    const Tracker::Entry* Tracker::Find(ID id) const
    {
        return const_cast<Tracker*>(this)->Find(id);
    }

    void Tracker::Clear()
    {
        map.clear();
        checker.clear();
        section.clear();
    }

    void Tracker::ClearSection(bool deleteEntries)
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

    bool Tracker::IsTracking(const void* obj) const
    {
        return Find(obj) != nullptr;
    }

    void Tracker::AddLookahead(void** position, ID id)
    {
        TrackingLookaheadEntry madeLookahead(position);

        auto found = FindLookaheadList(id);
        if (found == lookaheadMap.end())
        {
            LookaheadGroup group;
            group.list.push_back(std::move(madeLookahead));
            lookaheadMap.emplace(id, std::move(group));
        }
        else
            found->second.list.push_back(std::move(madeLookahead));
    }

    Tracker::LookaheadMap::iterator Tracker::FindLookaheadList(ID id)
    {
        return lookaheadMap.find(id);
    }

    void Tracker::LinkLookaheads(void* toObj, ID id)
    {
        auto foundLookAheadList = FindLookaheadList(id);
        if (foundLookAheadList != lookaheadMap.end())
        {
            for (auto &loop : foundLookAheadList->second.list)
                *loop.position = toObj;

            lookaheadMap.erase(foundLookAheadList);
        }
    }

    Tracker::ID Tracker::NextID() const
    {
        return (!map.empty()) ? static_cast<Tracker::ID>(map.size() + 1) : invalidTrackingID + 1;
    }
}