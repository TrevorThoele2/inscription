#include "TypeTracker.h"

namespace Inscription
{
    TypeTracker::TypeTracker(TypeTracker&& arg) : entryList(std::move(arg.entryList))
    {}

    TypeTracker& TypeTracker::operator=(TypeTracker&& arg)
    {
        entryList = std::move(arg.entryList);
        return *this;
    }

    TypeTracker::ID TypeTracker::Add(const Type& type)
    {
        {
            auto found = FindID(type);
            if (found.IsValid() && found != SpecialTypeTrackingID::NULLPTR)
                return *found;
        }

        auto id = NextID();
        entryList.push_back(Entry(id, type));
        return id;
    }

    bool TypeTracker::IsTypeIn(const Type& type) const
    {
        return FindID(type).IsValid();
    }

    Optional<TypeTracker::ID> TypeTracker::FindID(const Type& type) const
    {
        for (auto& loop : entryList)
            if (loop.type == type)
                return Optional<ID>(loop.id);

        return Optional<ID>();
    }

    TypeTracker::ID TypeTracker::NextID() const
    {
        return entryList.size();
    }
}