#pragma once

#include <list>
#include <unordered_map>
#include <functional>

#include "PointerDelegate.h"
#include "PointerSpecialID.h"

#include "ClassName.h"
#include "TrackingID.h"

#include "Type.h"

#include "PolymorphicTypeNotFound.h"

namespace Inscription
{
    class TrackerMap;

    template<class ScribeT>
    class PointerInput : public PointerDelegate
    {
    public:
        template<class T>
        void HandleOwning(T*& obj, ScribeT& scribe, TrackerMap& trackers);
        template<class T>
        void HandleUnowning(T*& obj, ScribeT& scribe, TrackerMap& trackers);

        template<class T>
        void Push(const ClassName& name);
    private:
        class PolymorphicEntry
        {
        public:
            const ClassName className;
            PointerInput* owner;

            template<class T>
            PolymorphicEntry(const ClassName& className, PointerInput& owner, Type<T>);

            void LoadObject(void*& obj, ScribeT& scribe, TrackerMap& trackers, bool owns);
        private:
            std::function<void(void*&, ScribeT&, TrackerMap&, bool)> _loadObject;
            std::function<void(TrackerMap&)> _enableTracking;
        };
    private:
        typedef std::list<PolymorphicEntry> PolymorphicEntryList;
        typedef typename PolymorphicEntryList::iterator EntryListIterator;
        PolymorphicEntryList list;

        typedef std::unordered_map<PolymorphicID, ClassName> PolymorphicIdToClassNames;
        typedef typename PolymorphicIdToClassNames::iterator PolymorphicIdToClassNameIterator;
        PolymorphicIdToClassNames polymorphicIdToClassNames;

        typedef std::unordered_map<ClassName, EntryListIterator> ClassNameToPolymorphicEntryMap;
        ClassNameToPolymorphicEntryMap classNameToPolymorphicEntry;
    private:
        void AddEntry(const PolymorphicEntry& add);
    private:
        template<class T>
        void Handle(T*& obj, ScribeT& scribe, TrackerMap& trackers, bool owns);

        template<class T>
        void HandleNullptr(T*& obj);
        template<class T, typename std::enable_if<!std::is_abstract<T>::value, int>::type = 0>
        void HandleNonpolymorphically(T*& obj, ScribeT& scribe, TrackerMap& trackers, PolymorphicID polymorphicId, bool owns);
        template<class T, typename std::enable_if<std::is_abstract<T>::value, int>::type = 0>
        void HandleNonpolymorphically(T*& obj, ScribeT& scribe, TrackerMap& trackers, PolymorphicID polymorphicId, bool owns);
        template<class T>
        void HandlePolymorphically(T*& obj, ScribeT& scribe, TrackerMap& trackers, PolymorphicID polymorphicId, bool owns);

        bool ShouldHandleNullptr(PolymorphicID polymorphicId) const;
        bool ShouldHandleNonpolymorphically(PolymorphicID polymorphicId) const;
    private:
        PolymorphicID LoadPolymorphicID(ScribeT& scribe);
        TrackingID LoadTrackingID(ScribeT& scribe);
        template<class T>
        void LoadTracked(T*& obj, ScribeT& scribe, TrackerMap& trackers, bool owns);
    private:
        PolymorphicIdToClassNameIterator EmplaceClassName(const ClassName& className);
    private:
        ClassName* FindClassName(PolymorphicID polymorphicId);
        EntryListIterator FindPolymorphicEntry(const ClassName& className);
    };

    template<class ScribeT>
    template<class T>
    void PointerInput<ScribeT>::HandleOwning(T*& obj, ScribeT& scribe, TrackerMap& trackers)
    {
        Handle(obj, scribe, trackers, true);
    }

    template<class ScribeT>
    template<class T>
    void PointerInput<ScribeT>::HandleUnowning(T*& obj, ScribeT& scribe, TrackerMap& trackers)
    {
        Handle(obj, scribe, trackers, false);
    }

    template<class ScribeT>
    template<class T>
    void PointerInput<ScribeT>::Push(const ClassName& name)
    {
        AddEntry(PolymorphicEntry(name, *this, Type<T>{}));
    }

    template<class ScribeT>
    template<class T>
    PointerInput<ScribeT>::PolymorphicEntry::PolymorphicEntry(const ClassName& className, PointerInput& owner, Type<T>) :
        className(className), owner(&owner)
    {
        _loadObject = [this](void*& obj, ScribeT& scribe, TrackerMap& trackers, bool owns)
        {
            this->owner->LoadTracked(reinterpret_cast<T*&>(obj), scribe, trackers, owns);
        };
    }

    template<class ScribeT>
    void PointerInput<ScribeT>::PolymorphicEntry::LoadObject(void*& obj, ScribeT& scribe, TrackerMap& trackers, bool owns)
    {
        _loadObject(obj, scribe, trackers, owns);
    }

    template<class ScribeT>
    void PointerInput<ScribeT>::AddEntry(const PolymorphicEntry& add)
    {
        list.push_back(add);
        classNameToPolymorphicEntry.emplace(add.className, --list.end());
    }

    template<class ScribeT>
    template<class T>
    void PointerInput<ScribeT>::Handle(T*& obj, ScribeT& scribe, TrackerMap& trackers, bool owns)
    {
        PolymorphicID polymorphicId = LoadPolymorphicID(scribe);

        if (ShouldHandleNullptr(polymorphicId))
            HandleNullptr(obj);
        else if (ShouldHandleNonpolymorphically(polymorphicId))
            HandleNonpolymorphically(obj, scribe, trackers, polymorphicId, owns);
        else
            HandlePolymorphically(obj, scribe, trackers, polymorphicId, owns);
    }

    template<class ScribeT>
    template<class T>
    void PointerInput<ScribeT>::HandleNullptr(T*& obj)
    {
        obj = nullptr;
    }

    template<class ScribeT>
    template<class T, typename std::enable_if<!std::is_abstract<T>::value, int>::type>
    void PointerInput<ScribeT>::HandleNonpolymorphically(T*& obj, ScribeT& scribe, TrackerMap& trackers, PolymorphicID polymorphicId, bool owns)
    {
        LoadTracked(obj, scribe, trackers, owns);
    }

    template<class ScribeT>
    template<class T, typename std::enable_if<std::is_abstract<T>::value, int>::type>
    void PointerInput<ScribeT>::HandleNonpolymorphically(T*& obj, ScribeT& scribe, TrackerMap& trackers, PolymorphicID polymorphicId, bool owns)
    {}

    template<class ScribeT>
    template<class T>
    void PointerInput<ScribeT>::HandlePolymorphically(T*& obj, ScribeT& scribe, TrackerMap& trackers, PolymorphicID polymorphicId, bool owns)
    {
        ClassName* className = nullptr;
        // Find the name
        {
            auto foundClassName = FindClassName(polymorphicId);
            if (!foundClassName)
            {
                ClassName loadedClassName;
                scribe.AsInput()->Load(loadedClassName);

                auto emplacedPolymorphicEntry = EmplaceClassName(loadedClassName);
                if (emplacedPolymorphicEntry == polymorphicIdToClassNames.end())
                    throw PolymorphicTypeNotFound();

                className = &emplacedPolymorphicEntry->second;
            }
            else
                className = foundClassName;
        }

        auto polymorphicEntry = FindPolymorphicEntry(*className);
        // Loads the tracker ID
        polymorphicEntry->LoadObject(reinterpret_cast<void*&>(obj), scribe, trackers, owns);
    }

    template<class ScribeT>
    template<class T>
    void PointerInput<ScribeT>::LoadTracked(T*& obj, ScribeT& scribe, TrackerMap& trackers, bool owns)
    {
        // Load ID always and selectively load the object after
        TrackingID id = LoadTrackingID(scribe);
        if (id == invalidTrackingID)
        {
            obj = nullptr;
            return;
        }

        auto foundObject = trackers.FindObject<T>(id);
        // Load object
        if (foundObject)
            // Set object if it's here
            obj = foundObject;
        else
        {
            if (owns)
            {
                // Create new object if it isn't in here previously
                Construct(scribe, obj);
                // Place new object into the trackers
                trackers.Add(obj);
            }
            else
                trackers.AddLookAhead(&obj);
        }
    }

    template<class ScribeT>
    bool PointerInput<ScribeT>::ShouldHandleNullptr(PolymorphicID id) const
    {
        return id == PointerSpecialID::NULLPTR;
    }

    template<class ScribeT>
    bool PointerInput<ScribeT>::ShouldHandleNonpolymorphically(PolymorphicID id) const
    {
        return id == PointerSpecialID::HANDLE_NONPOLYMORPHICALLY;
    }

    template<class ScribeT>
    PolymorphicID PointerInput<ScribeT>::LoadPolymorphicID(ScribeT& scribe)
    {
        PolymorphicID loaded = PointerSpecialID::NULLPTR;
        scribe.AsInput()->Load(loaded);
        return loaded;
    }

    template<class ScribeT>
    TrackingID PointerInput<ScribeT>::LoadTrackingID(ScribeT& scribe)
    {
        TrackingID loaded = 0;
        scribe.AsInput()->Load(loaded);
        return loaded;
    }

    template<class ScribeT>
    typename PointerInput<ScribeT>::PolymorphicIdToClassNameIterator PointerInput<ScribeT>::EmplaceClassName(const ClassName& className)
    {
        return polymorphicIdToClassNames.emplace(nextID++, className).first;
    }

    template<class ScribeT>
    ClassName* PointerInput<ScribeT>::FindClassName(PolymorphicID polymorphicId)
    {
        auto found = polymorphicIdToClassNames.find(polymorphicId);
        if (found == polymorphicIdToClassNames.end())
            return nullptr;

        return &found->second;
    }

    template<class ScribeT>
    typename PointerInput<ScribeT>::EntryListIterator PointerInput<ScribeT>::FindPolymorphicEntry(const ClassName& className)
    {
        auto found = classNameToPolymorphicEntry.find(className);
        if (found == classNameToPolymorphicEntry.end())
            return list.end();

        return found->second;
    }
}