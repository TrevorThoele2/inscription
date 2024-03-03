
#pragma once

#include <vector>
#include <type_traits>
#include <memory>
#include <unordered_set>

#include "TrackerID.h"
#include "TrackingTraits.h"
#include "TypeTable.h"
#include "Const.h"

namespace inscription
{
    namespace detail
    {
        // The definition is moved down below TrackingGroup
        class TrackerInstancer
        {
        private:
            template<class T>
            struct Type
            {
                static Type instance;

                Type()
                {
                    (void)this;
                    TrackerInstancer::Instance().types.push_back(typeid(T));
                }

                Type(const Type &arg) = delete;
                Type& operator=(const Type &arg) = delete;
            };
        private:
            std::vector<std::type_index> types;

            TrackerInstancer() { (void)this; }
            TrackerInstancer(const TrackerInstancer &arg) = delete;
            TrackerInstancer& operator=(const TrackerInstancer &arg) = delete;
        public:
            static TrackerInstancer& Instance();
            template<class Group>
            static void Push(Group &group);
            template<class T>
            static void Enable(std::true_type);
            template<class T>
            static void Enable(std::false_type) {}
        };
    }

    class TrackingEntry
    {
    public:
        typedef TrackerID ID;
    private:
        ID id;
        void *obj;
    public:
        TrackingEntry();
        TrackingEntry(ID id, void *obj);
        TrackingEntry(const TrackingEntry &arg);
        TrackingEntry& operator=(const TrackingEntry &arg);
        ID GetID();
        void* Get();
        const void* Get() const;
    };

    class Tracker
    {
    public:
        typedef TrackerID ID;
        typedef TrackingEntry Entry;
    private:
        typedef std::unordered_map<ID, Entry> Map;
        typedef typename Map::iterator iterator;

        typedef std::unordered_map<const void*, iterator> Checker;
        typedef std::unordered_map<const void*, iterator> Section;
    private:
        Map map;
        Checker checker;

        Section section;

        std::pair<bool, Entry&> Add(ID id, void *add, bool section);
        ID NextID() const;
    public:
        Tracker() = default;
        Tracker(const Tracker &arg) = default;
        Tracker(Tracker &&arg);
        Tracker& operator=(const Tracker &arg) = default;
        Tracker& operator=(Tracker &&arg);

        std::pair<bool, Entry&> Add(void *add, bool section);
        void ReplaceObject(void *here, void *newObj, bool section);
        Entry* Find(void *find);
        Entry* Find(ID id);
        void Clear();

        void ClearSection(bool deleteEntries);
    };

    class TrackerGroup
    {
    public:
        typedef TrackerID ID;
        static constexpr ID nullID = 0;
    private:
        typedef TypeTable<Tracker> Table;
        friend detail::TrackerInstancer;
    private:
        bool active;
        Table table;

        bool usingSection;
    public:
        TrackerGroup();
        TrackerGroup(const TrackerGroup &arg) = default;
        TrackerGroup(TrackerGroup &&arg);
        TrackerGroup& operator=(const TrackerGroup &arg) = default;
        TrackerGroup& operator=(TrackerGroup &&arg);
        void SetActive(bool set = true);
        bool IsActive() const;

        // Returns true in first if the entry is unique (hasn't been added previously)
        template<class T>
        std::pair<bool, Tracker::Entry*> Add(T *add);
        std::pair<bool, Tracker::Entry*> Add(void *add, const std::type_index &type, std::true_type);
        std::pair<bool, Tracker::Entry*> Add(void *add, const std::type_index &type, std::false_type);

        template<class T>
        bool IsTypeInside() const;
        template<class T>
        static void AttemptEnableSelectiveTracking();
        template<class T>
        static void AttemptEnableAlwaysTracking();
        template<class T>
        T* Find(ID id);
        template<class T>
        void ReplaceObject(T &here, T &newObj);

        void Clear();

        void StartSection();
        void StopSection();
        void ClearSection();
    };

    template<class T>
    std::pair<bool, Tracker::Entry*> TrackerGroup::Add(T *add)
    {
        AttemptEnableAlwaysTracking<T>();

        if (!add)
            return std::pair<bool, Tracker::Entry*>(false, nullptr);

        // Only add this object if the object's type trait is not set to never track
        return Add(RemoveConst(add), typeid(*add), std::integral_constant<bool, TrackPointer<typename std::remove_cv<T>::type>::value != TRACK_NEVER>{});
    }

    template<class T>
    bool TrackerGroup::IsTypeInside() const
    {
        return table.Find(std::type_index(typeid(T))) != nullptr;
    }

    template<class T>
    void TrackerGroup::AttemptEnableSelectiveTracking()
    {
        // Enable tracking if object's tracking trait is selectively and is not abstract
        detail::TrackerInstancer::Enable<T>(std::integral_constant<bool, !std::is_abstract<T>::value && (TrackPointer<typename std::remove_cv<T>::type>::value == TRACK_SELECTIVELY)>::type());
    }

    template<class T>
    void TrackerGroup::AttemptEnableAlwaysTracking()
    {
        detail::TrackerInstancer::Enable<typename std::remove_cv<T>::type>(std::integral_constant<bool, TrackPointer<typename std::remove_cv<T>::type>::value == TRACK_ALWAYS>::type());
    }

    template<class T>
    T* TrackerGroup::Find(ID id)
    {
        if (id == nullID)
            return nullptr;

        auto tableFound = table.Find(std::type_index(typeid(T)));
        if (!tableFound)
            return nullptr;

        auto found = tableFound->Find(id);
        if (found)
            return static_cast<T*>(found->Get());
        else
            return nullptr;
    }

    template<class T>
    void TrackerGroup::ReplaceObject(T &here, T &newObj)
    {
        if (typeid(here) != typeid(newObj))
            return;

        auto tableFound = table.Find(typeid(here));
        if (!tableFound)
            return;

        tableFound->ReplaceObject(&here, &newObj, usingSection);
    }

    class Scribe;
    class TrackingChangerStack
    {
    private:
        bool prev;
        Scribe &scribe;
    public:
        // ChangeTo is the value that the scribe's tracking will be changed into
        TrackingChangerStack(Scribe &scribe, bool changeTo);
        ~TrackingChangerStack();
    };

    namespace detail
    {
        template<class T>
        TrackerInstancer::Type<T> TrackerInstancer::Type<T>::instance;

        template<class Group>
        void TrackerInstancer::Push(Group &group)
        {
            for (auto &loop : Instance().types)
                group.table.Add(inscription::Tracker(), loop);
        }

        template<class T>
        void TrackerInstancer::Enable(std::true_type)
        {
            Type<T>::instance;
        }
    }
}