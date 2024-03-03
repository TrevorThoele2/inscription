
#include "Tracking.h"
#include "Scribe.h"

namespace Inscription
{
    TrackingEntry::TrackingEntry() : id(TrackerGroup::nullID), obj(nullptr), hasBeenSerialized(false)
    {}

    TrackingEntry::TrackingEntry(ID id, void *obj) : id(id), obj(obj), hasBeenSerialized(false)
    {}

    TrackingEntry::TrackingEntry(const TrackingEntry &arg) : id(arg.id), obj(arg.obj), hasBeenSerialized(arg.hasBeenSerialized)
    {}

    TrackingLookAheadEntry::TrackingLookAheadEntry(void** position) : position(position)
    {}

    TrackingLookAheadGroup::TrackingLookAheadGroup(TrackingLookAheadGroup&& arg) : list(std::move(arg.list))
    {}

    TrackingLookAheadGroup& TrackingLookAheadGroup::operator=(TrackingLookAheadGroup&& arg)
    {
        list = std::move(arg.list);
        return *this;
    }

    Tracker::Tracker(Tracker &&arg) : map(std::move(arg.map)), checker(std::move(arg.checker)), section(std::move(arg.section))
    {}

    Tracker& Tracker::operator=(Tracker &&arg)
    {
        map = std::move(arg.map);
        checker = std::move(arg.checker);
        section = std::move(arg.section);
        return *this;
    }

    Tracker::ID Tracker::Add(void *add, bool section)
    {
        if (IsTracking(add))
        {
            auto id = Find(add)->id;
            LinkLookAheads(add, id);
            return id;
        }

        return Add(add, section, NextID());
    }

    Tracker::ID Tracker::Add(void *add, bool section, ID id)
    {
        auto emplaced = map.emplace(id, Entry(id, add));
        checker.emplace(add, emplaced.first);
        if (section)
            this->section.emplace(add, emplaced.first);

        LinkLookAheads(add, id);
        return id;
    }

    void Tracker::AddLookAhead(void** position)
    {
        if (IsTracking(*position))
            return;

        AddLookAhead(position, NextID());
    }

    void Tracker::ReplaceObject(void *here, void *newObj, bool section)
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

    Tracker::Entry* Tracker::Find(const void *find)
    {
        auto checkerFound = checker.find(find);
        if (checkerFound == checker.end())
            return nullptr;
        else
            return &checkerFound->second->second;
    }

    const Tracker::Entry* Tracker::Find(const void *find) const
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

    void Tracker::AddLookAhead(void** position, ID id)
    {
        TrackingLookAheadEntry madeLookAhead(position);

        auto found = FindLookAheadList(id);
        if (found == lookAheads.end())
        {
            LookAheadGroup group;
            group.list.push_back(std::move(madeLookAhead));
            lookAheads.emplace(id, std::move(group));
        }
        else
            found->second.list.push_back(std::move(madeLookAhead));
    }

    Tracker::LookAheadMap::iterator Tracker::FindLookAheadList(ID id)
    {
        return lookAheads.find(id);
    }

    void Tracker::LinkLookAheads(void* toObj, ID id)
    {
        auto foundLookAheadList = FindLookAheadList(id);
        if (foundLookAheadList != lookAheads.end())
        {
            for (auto &loop : foundLookAheadList->second.list)
                *loop.position = toObj;

            lookAheads.erase(foundLookAheadList);
        }
    }

    Tracker::ID Tracker::NextID() const
    {
        if (!map.empty())
            return static_cast<Tracker::ID>(map.size() + 1);
        else
            return TrackerGroup::nullID + 1;
    }

    TrackerGroup::TrackerGroup() : active(true), usingSection(false)
    {
        RegisteredTypes::PushToTracking(*this);
    }

    TrackerGroup::TrackerGroup(TrackerGroup &&arg) : active(arg.active), table(std::move(arg.table)), usingSection(arg.usingSection)
    {}

    TrackerGroup& TrackerGroup::operator=(TrackerGroup &&arg)
    {
        active = arg.active;
        table = std::move(arg.table);

        usingSection = arg.usingSection;
        return *this;
    }

    void TrackerGroup::SetActive(bool set)
    {
        active = set;
    }

    bool TrackerGroup::IsActive() const
    {
        return active;
    }

    TrackerGroup::ID TrackerGroup::Add(void *add, const std::type_index &type)
    {
        // Find the tracker
        auto found = table.Find(type);
        if (!found)
            return nullID;

        if (!IsActive())
            return nullID;

        return found->Add(add, usingSection);
    }

    void TrackerGroup::Clear()
    {
        usingSection = false;
        for (auto &loop : table)
            loop.second.Clear();
    }

    void TrackerGroup::StartSection()
    {
        usingSection = true;
    }

    void TrackerGroup::StopSection()
    {
        usingSection = false;
        for (auto &loop : table)
            loop.second.ClearSection(false);
    }

    void TrackerGroup::ClearSection()
    {
        for (auto &loop : table)
            loop.second.ClearSection(true);
    }

    TrackingChangerStack::TrackingChangerStack(Scribe &scribe, bool changeTo) : scribe(scribe), prev(scribe.TrackObjects(changeTo))
    {}

    TrackingChangerStack::~TrackingChangerStack()
    {
        scribe.TrackObjects(prev);
    }
}