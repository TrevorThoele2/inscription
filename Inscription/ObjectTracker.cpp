#include "ObjectTracker.h"

namespace Inscription
{
    ObjectTracker::ObjectTracker() : active(true), usingSection(false)
    {}

    ObjectTracker::ObjectTracker(ObjectTracker&& arg) : active(arg.active), typeMap(std::move(arg.typeMap)), usingSection(arg.usingSection)
    {}

    ObjectTracker& ObjectTracker::operator=(ObjectTracker&& arg)
    {
        active = arg.active;
        typeMap = std::move(arg.typeMap);

        usingSection = arg.usingSection;
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

    Optional<ObjectTracker::ID> ObjectTracker::Add(void* add, const std::type_index& type)
    {
        auto found = typeMap.Find(type);
        if (!found)
            return Optional<ID>();

        if (!IsActive())
            return Optional<ID>();

        return found->Add(add, usingSection);
    }

    void ObjectTracker::Clear()
    {
        usingSection = false;
        for (auto& loop : typeMap)
            loop.second.Clear();
    }

    void ObjectTracker::StartSection()
    {
        usingSection = true;
    }

    void ObjectTracker::StopSection()
    {
        usingSection = false;
        for (auto& loop : typeMap)
            loop.second.ClearSection(false);
    }

    void ObjectTracker::ClearSection()
    {
        for (auto& loop : typeMap)
            loop.second.ClearSection(true);
    }
}