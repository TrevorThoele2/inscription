#pragma once

#include <typeindex>
#include <vector>

#include "TypeTrackerEntry.h"
#include "TrackingID.h"

#include "Optional.h"

namespace Inscription
{
    class TypeTracker
    {
    public:
        using ID = TrackingID;
        using Type = std::type_index;
    public:
        TypeTracker() = default;
        TypeTracker(TypeTracker&& arg) noexcept;

        TypeTracker& operator=(TypeTracker&& arg) noexcept;

        template<class T>
        ID Add();
        ID Add(const Type& type);
        ID Add(const Type& type, ID id);

        void SignalSavedConstruction(ID id);
        [[nodiscard]] bool HasSavedConstruction(ID id) const;

        template<class T>
        [[nodiscard]] bool IsTypeIn() const;
        [[nodiscard]] bool IsTypeIn(const Type& type) const;
        [[nodiscard]] bool IsIDIn(ID id) const;
        template<class T>
        [[nodiscard]] Optional<ID> FindID() const;
        [[nodiscard]] Optional<ID> FindID(const Type& type) const;

        [[nodiscard]] Optional<Type> FindType(ID id) const;
    private:
        using Entry = TypeTrackerEntry;
        using EntryList = std::vector<Entry>;
        EntryList entryList;

        [[nodiscard]] Entry* FindEntry(ID id);
        [[nodiscard]] const Entry* FindEntry(ID id) const;
    private:
        [[nodiscard]] ID NextID() const;
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