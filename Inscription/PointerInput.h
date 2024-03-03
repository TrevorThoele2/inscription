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
    class BinaryScribe;
    class TrackerMap;

    class PointerInput : public PointerDelegate
    {
    public:
        template<class T>
        void HandleOwning(T*& obj, BinaryScribe& scribe, TrackerMap& trackers);
        template<class T>
        void HandleUnowning(T*& obj, BinaryScribe& scribe, TrackerMap& trackers);

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

            void LoadObject(void*& obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns);
        private:
            std::function<void(void*&, BinaryScribe&, TrackerMap&, bool)> _loadObject;
            std::function<void(TrackerMap&)> _enableTracking;
        };
    private:
        typedef std::list<PolymorphicEntry> PolymorphicEntryList;
        PolymorphicEntryList list;

        typedef std::unordered_map<PolymorphicID, ClassName> PolymorphicIdToClassNames;
        PolymorphicIdToClassNames polymorphicIdToClassNames;

        typedef std::unordered_map<ClassName, PolymorphicEntryList::iterator> ClassNameToPolymorphicEntryMap;
        ClassNameToPolymorphicEntryMap classNameToPolymorphicEntry;
    private:
        void AddEntry(const PolymorphicEntry& add);
    private:
        template<class T>
        void Handle(T*& obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns);

        template<class T>
        void HandleNullptr(T*& obj);
        template<class T, typename std::enable_if<!std::is_abstract<T>::value, int>::type = 0>
        void HandleNonpolymorphically(T*& obj, BinaryScribe& scribe, TrackerMap& trackers, PolymorphicID polymorphicId, bool owns);
        template<class T, typename std::enable_if<std::is_abstract<T>::value, int>::type = 0>
        void HandleNonpolymorphically(T*& obj, BinaryScribe& scribe, TrackerMap& trackers, PolymorphicID polymorphicId, bool owns);
        template<class T>
        void HandlePolymorphically(T*& obj, BinaryScribe& scribe, TrackerMap& trackers, PolymorphicID polymorphicId, bool owns);

        bool ShouldHandleNullptr(PolymorphicID polymorphicId) const;
        bool ShouldHandleNonpolymorphically(PolymorphicID polymorphicId) const;
    private:
        PolymorphicID LoadPolymorphicID(BinaryScribe& scribe);
        TrackingID LoadTrackingID(BinaryScribe& scribe);
        template<class T>
        void LoadTracked(T*& obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns);
    private:
        PolymorphicIdToClassNames::iterator EmplaceClassName(const ClassName& className);
    private:
        ClassName* FindClassName(PolymorphicID polymorphicId);
        PolymorphicEntryList::iterator FindPolymorphicEntry(const ClassName& className);
    };

    template<class T>
    void PointerInput::HandleOwning(T*& obj, BinaryScribe& scribe, TrackerMap& trackers)
    {
        Handle(obj, scribe, trackers, true);
    }

    template<class T>
    void PointerInput::HandleUnowning(T*& obj, BinaryScribe& scribe, TrackerMap& trackers)
    {
        Handle(obj, scribe, trackers, false);
    }

    template<class T>
    void PointerInput::Push(const ClassName& name)
    {
        AddEntry(PolymorphicEntry(name, *this, Type<T>{}));
    }

    template<class T>
    PointerInput::PolymorphicEntry::PolymorphicEntry(const ClassName& className, PointerInput& owner, Type<T>) :
        className(className), owner(&owner)
    {
        _loadObject = [this](void*& obj, Scribe& scribe, TrackerMap& trackers, bool owns)
        {
            this->owner->LoadTracked(reinterpret_cast<T*&>(obj), scribe, trackers, owns);
        };
    }

    template<class T>
    void PointerInput::Handle(T*& obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns)
    {
        PolymorphicID polymorphicId = LoadPolymorphicID(scribe);

        if (ShouldHandleNullptr(polymorphicId))
            HandleNullptr(obj);
        else if (ShouldHandleNonpolymorphically(polymorphicId))
            HandleNonpolymorphically(obj, scribe, trackers, polymorphicId, owns);
        else
            HandlePolymorphically(obj, scribe, trackers, polymorphicId, owns);
    }

    template<class T>
    void PointerInput::HandleNullptr(T*& obj)
    {
        obj = nullptr;
    }

    template<class T, typename std::enable_if<!std::is_abstract<T>::value, int>::type>
    void PointerInput::HandleNonpolymorphically(T*& obj, BinaryScribe &scribe, TrackerMap& trackers, PolymorphicID polymorphicId, bool owns)
    {
        LoadTracked(obj, scribe, trackers, owns);
    }

    template<class T, typename std::enable_if<std::is_abstract<T>::value, int>::type>
    void PointerInput::HandleNonpolymorphically(T*& obj, BinaryScribe &scribe, TrackerMap& trackers, PolymorphicID polymorphicId, bool owns)
    {}

    template<class T>
    void PointerInput::HandlePolymorphically(T*& obj, BinaryScribe& scribe, TrackerMap& trackers, PolymorphicID polymorphicId, bool owns)
    {
        ClassName* className = nullptr;
        // Find the name
        {
            auto foundClassName = FindClassName(polymorphicId);
            if (!foundClassName)
            {
                ClassName loadedClassName;
                scribe.Load(loadedClassName);

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

    template<class T>
    void PointerInput::LoadTracked(T*& obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns)
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
}