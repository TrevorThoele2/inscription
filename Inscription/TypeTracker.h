#pragma once

#include <typeindex>
#include <vector>

#include "TypeTrackerEntry.h"
#include "TrackingID.h"
#include "SpecialTypeTrackingID.h"

#include "Optional.h"

namespace Inscription
{
    class TypeTracker
    {
    public:
        typedef TrackingID ID;
        typedef std::type_index Type;
    public:
        TypeTracker() = default;
        TypeTracker(TypeTracker&& arg);

        TypeTracker& operator=(TypeTracker&& arg);

        template<class T>
        ID Add();
        ID Add(const Type& type);

        template<class T>
        bool IsTypeIn() const;
        bool IsTypeIn(const Type& type) const;
        template<class T>
        Optional<ID> FindID() const;
        Optional<ID> FindID(const Type& type) const;
    private:
        typedef TypeTrackerEntry Entry;
        typedef std::vector<Entry> EntryList;
        EntryList entryList;

        ID NextID() const;
    private:
        template<class T>
        static Type TypeFor();
    };

    template<class T>
    TypeTracker::ID TypeTracker::Add()
    {
        return Add(TypeFor<T>());
    }

    template<class T>
    bool TypeTracker::IsTypeIn() const
    {
        return IsTypeIn(TypeFor<T>());
    }

    template<class T>
    Optional<TypeTracker::ID> TypeTracker::FindID() const
    {
        return FindID(TypeFor<T>());
    }

    template<class T>
    TypeTracker::Type TypeTracker::TypeFor()
    {
        return typeid(T);
    }
}