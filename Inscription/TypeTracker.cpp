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
        return Add(type, NextID());
    }

    TypeTracker::ID TypeTracker::Add(const Type& type, ID id)
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
        auto found = FindEntry(id);
        if (!found)
            return;

        found->hasSavedConstruction = true;
    }

    bool TypeTracker::HasSavedConstruction(ID id) const
    {
        auto found = FindEntry(id);
        if (!found)
            return false;

        return found->hasSavedConstruction;
    }

    bool TypeTracker::IsTypeIn(const Type& type) const
    {
        return FindID(type).IsValid();
    }

    bool TypeTracker::IsIDIn(ID id) const
    {
        for (auto& loop : entryList)
            if (loop.id == id)
                return true;

        return false;
    }

    Optional<TypeTracker::ID> TypeTracker::FindID(const Type& type) const
    {
        for (auto& loop : entryList)
            if (loop.type == type)
                return Optional<ID>(loop.id);

        return Optional<ID>();
    }

    Optional<TypeTracker::Type> TypeTracker::FindType(ID id) const
    {
        for (auto& loop : entryList)
            if (loop.id == id)
                return Optional<Type>(loop.type);

        return Optional<Type>();
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