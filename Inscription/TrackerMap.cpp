#include "TrackerMap.h"

#include "RegisteredTypes.h"

namespace Inscription
{
    TrackerMap::TrackerMap() : active(true), usingSection(false)
    {
        RegisteredTypes::CopyRegisteredTo(*this);
    }

    TrackerMap::TrackerMap(TrackerMap&& arg) : active(arg.active), table(std::move(arg.table)), usingSection(arg.usingSection)
    {}

    TrackerMap& TrackerMap::operator=(TrackerMap&& arg)
    {
        active = arg.active;
        table = std::move(arg.table);

        usingSection = arg.usingSection;
        return *this;
    }

    void TrackerMap::SetActive(bool set)
    {
        active = set;
    }

    bool TrackerMap::IsActive() const
    {
        return active;
    }

    TrackerMap::ID TrackerMap::Add(void* add, const std::type_index& type)
    {
        // Find the tracker
        auto found = table.Find(type);
        if (!found)
            return invalidTrackingID;

        if (!IsActive())
            return invalidTrackingID;

        return found->Add(add, usingSection);
    }

    void TrackerMap::Clear()
    {
        usingSection = false;
        for (auto& loop : table)
            loop.second.Clear();
    }

    void TrackerMap::StartSection()
    {
        usingSection = true;
    }

    void TrackerMap::StopSection()
    {
        usingSection = false;
        for (auto& loop : table)
            loop.second.ClearSection(false);
    }

    void TrackerMap::ClearSection()
    {
        for (auto& loop : table)
            loop.second.ClearSection(true);
    }
}