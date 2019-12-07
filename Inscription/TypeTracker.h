#pragma once

#include <typeindex>
#include <vector>
#include <optional>

#include "TypeTrackerEntry.h"
#include "TrackingID.h"

namespace Inscription
{
    class TypeTracker
    {
    public:
        using ID = TrackingID;
    public:
        TypeTracker() = default;
        TypeTracker(TypeTracker&& arg) noexcept;

        TypeTracker& operator=(TypeTracker&& arg) noexcept;

        template<class T>
        ID Add();
        ID Add(const std::type_index& type);
        ID Add(const std::type_index& type, ID id);

        void SignalSavedConstruction(ID id);
        [[nodiscard]] bool HasSavedConstruction(ID id) const;

        template<class T>
        [[nodiscard]] bool IsTypeIn() const;
        [[nodiscard]] bool IsTypeIn(const std::type_index& type) const;
        [[nodiscard]] bool IsIDIn(ID id) const;
        template<class T>
        [[nodiscard]] std::optional<ID> FindID() const;
        [[nodiscard]] std::optional<ID> FindID(const std::type_index& type) const;

        [[nodiscard]] std::optional<std::type_index> FindType(ID id) const;
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
        static std::type_index TypeFor();
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
    std::optional<TypeTracker::ID> TypeTracker::FindID() const
    {
        return FindID(TypeFor<T>());
    }

    template<class T>
    std::type_index TypeTracker::TypeFor()
    {
        return typeid(T);
    }
}