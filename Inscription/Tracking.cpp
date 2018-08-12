
#include "Tracking.h"
#include "Scribe.h"

namespace Inscription
{
    namespace detail
    {
        TrackerInstancer& TrackerInstancer::Instance()
        {
            static TrackerInstancer instance;
            return instance;
        }
    }

    TrackingEntry::TrackingEntry() : id(TrackerGroup::nullID), obj(nullptr)
    {}

    TrackingEntry::TrackingEntry(ID id, void *obj) : id(id), obj(obj)
    {}

    TrackingEntry::TrackingEntry(const TrackingEntry &arg) : id(arg.id), obj(arg.obj)
    {}

    TrackingEntry& TrackingEntry::operator=(const TrackingEntry &arg)
    {
        id = arg.id;
        obj = arg.obj;
        return *this;
    }

    TrackingEntry::ID TrackingEntry::GetID()
    {
        return id;
    }

    void* TrackingEntry::Get()
    {
        return obj;
    }

    const void* TrackingEntry::Get() const
    {
        return obj;
    }

    std::pair<bool, Tracker::Entry&> Tracker::Add(ID id, void *add, bool section)
    {
        typedef std::pair<bool, Tracker::Entry&> Return;
        if (!checker.empty())
        {
            auto checkerFound = checker.find(add);
            if (checkerFound != checker.end())
                return Return(false, checkerFound->second->second);
        }

        auto emplaced = map.emplace(id, Entry(id, add));
        checker.emplace(add, emplaced.first);
        if (section)
            this->section.emplace(add, emplaced.first);

        return Return(emplaced.second, emplaced.first->second);
    }

    Tracker::ID Tracker::NextID() const
    {
        if (!map.empty())
            return static_cast<Tracker::ID>(map.size() + 1);
        else
            return TrackerGroup::nullID + 1;
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

    std::pair<bool, Tracker::Entry&> Tracker::Add(void *add, bool section)
    {
        return Add(NextID(), add, section);
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
            Add(id, newObj, section);
        }
    }

    Tracker::Entry* Tracker::Find(void *find)
    {
        auto checkerFound = checker.find(find);
        if (checkerFound == checker.end())
            return nullptr;
        else
            return &checkerFound->second->second;
    }

    Tracker::Entry* Tracker::Find(ID id)
    {
        auto found = map.find(id);
        if (found == map.end())
            return nullptr;
        else
            return &found->second;
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
                checker.erase(loop.second->second.Get());
                map.erase(loop.second);
            }
        }

        section.clear();
    }

    TrackerGroup::TrackerGroup() : active(true), usingSection(false)
    {
        detail::TrackerInstancer::Push(*this);
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

    std::pair<bool, Tracker::Entry*> TrackerGroup::Add(void *add, const std::type_index &type, std::true_type)
    {
        typedef std::pair<bool, Tracker::Entry*> Return;

        static Tracker::Entry nullEntry(nullID, nullptr);
        static const Return falseRet(false, &nullEntry);

        // Find the tracker
        auto found = table.Find(type);
        if (!found)
            return falseRet;

        if (!IsActive())
            return Return(false, found->Find(add));
        else
        {
            auto &added = found->Add(add, usingSection);
            return Return(added.first, &added.second);
        }
    }

    std::pair<bool, Tracker::Entry*> TrackerGroup::Add(void *add, const std::type_index &type, std::false_type)
    {
        return std::pair<bool, Tracker::Entry*>(false, nullptr);
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