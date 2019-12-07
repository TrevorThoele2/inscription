#include "TypeTracker.h"

namespace Inscription
{
    TypeTracker::TypeTracker(TypeTracker&& arg) noexcept : entryList(std::move(arg.entryList))
    {}

    TypeTracker& TypeTracker::operator=(TypeTracker&& arg) noexcept
    {
        entryList = std::move(arg.entryList);
        return *this;
    }

    TypeTracker::ID TypeTracker::Add(const std::type_index& type)
    {
        return Add(type, NextID());
    }

    TypeTracker::ID TypeTracker::Add(const std::type_index& type, ID id)
    {
        {
            for (auto& loop : entryList)
                if (loop.type == type || loop.id == id)
                    return loop.id;
        }

        entryList.push_back(Entry(id, type));
        return id;
    }

    void TypeTracker::SignalSavedConstruction(ID id)
    {
        const auto found = FindEntry(id);
        if (!found)
            return;

        found->hasSavedConstruction = true;
    }

    bool TypeTracker::HasSavedConstruction(ID id) const
    {
        const auto found = FindEntry(id);
        if (!found)
            return false;

        return found->hasSavedConstruction;
    }

    bool TypeTracker::IsTypeIn(const std::type_index& type) const
    {
        return FindID(type).has_value();
    }

    bool TypeTracker::IsIDIn(ID id) const
    {
        for (auto& loop : entryList)
            if (loop.id == id)
                return true;

        return false;
    }

    std::optional<TypeTracker::ID> TypeTracker::FindID(const std::type_index& type) const
    {
        for (auto& loop : entryList)
            if (loop.type == type)
                return { loop.id };

        return {};
    }

    std::optional<std::type_index> TypeTracker::FindType(ID id) const
    {
        for (auto& loop : entryList)
            if (loop.id == id)
                return { loop.type };

        return {};
    }

    TypeTracker::Entry* TypeTracker::FindEntry(ID id)
    {
        for (auto& loop : entryList)
            if (loop.id == id)
                return &loop;

        return nullptr;
    }

    const TypeTracker::Entry* TypeTracker::FindEntry(ID id) const
    {
        for (auto& loop : entryList)
            if (loop.id == id)
                return &loop;

        return nullptr;
    }

    TypeTracker::ID TypeTracker::NextID() const
    {
        return entryList.size();
    }
}