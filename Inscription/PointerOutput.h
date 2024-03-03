#pragma once

#include <list>
#include <unordered_map>
#include <typeindex>
#include <functional>

#include "PointerDelegate.h"
#include "PointerSpecialID.h"

#include "ClassName.h"
#include "TrackingID.h"

#include "Const.h"
#include "Type.h"

#include "PolymorphicTypeNotFound.h"

namespace Inscription
{
    class BinaryScribe;
    class TrackerMap;

    class PointerOutput : public PointerDelegate
    {
    public:
        template<class T>
        void HandleOwning(T* obj, BinaryScribe& scribe, TrackerMap& trackers);
        template<class T>
        void HandleUnowning(T* obj, BinaryScribe& scribe, TrackerMap& trackers);

        template<class T>
        void Push(const ClassName& name);
    private:
        class PolymorphicEntry
        {
        public:
            PolymorphicID id;
            const ClassName className;
            PointerOutput* owner;

            template<class T>
            PolymorphicEntry(const ClassName& className, PointerOutput& owner, Type<T>);

            void SaveID(BinaryScribe& scribe);
            void SaveClassName(BinaryScribe& scribe);

            void SaveObject(const void* obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns);
            TrackingID FindFromTracker(const void* obj, TrackerMap& trackers);
            TrackingID Track(const void* obj, TrackerMap& trackers);
        private:
            std::function<void(const void*, BinaryScribe&, TrackerMap&, bool)> _saveObject;
            std::function<TrackingID(const void*, TrackerMap&)> _findFromTracker;
            std::function<TrackingID(const void*, TrackerMap&)> _track;
        };
    private:
        typedef std::list<PolymorphicEntry> PolymorphicEntryList;
        PolymorphicEntryList list;

        typedef std::unordered_map<std::type_index, PolymorphicEntryList::iterator> TypeMap;
        TypeMap types;
    private:
        void AddEntry(const PolymorphicEntry& add, const std::type_index& type);
    private:
        template<class T>
        void Handle(T* obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns);

        void HandleNullptr(BinaryScribe& scribe);
        template<class T, typename std::enable_if<!std::is_abstract<T>::value, int>::type = 0>
        void HandleNonpolymorphically(T* obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns);
        template<class T, typename std::enable_if<std::is_abstract<T>::value, int>::type = 0>
        void HandleNonpolymorphically(T* obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns);
        template<class T>
        void HandlePolymorphically(T* obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns);

        template<class T>
        bool ShouldHandleNullptr(T* obj);
        template<class T>
        bool ShouldHandleNonpolymorphically(T* obj);
    private:
        template<class T>
        void SaveTracked(T* obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns);
    private:
        template<class T>
        std::type_index TypeOf(T* obj);
        template<class T>
        PolymorphicEntry* FindRequiredPolymorphicEntry(T* obj);
    };

    template<class T>
    void PointerOutput::HandleOwning(T* obj, BinaryScribe& scribe, TrackerMap& trackers)
    {
        Handle(obj, scribe, trackers, true);
    }

    template<class T>
    void PointerOutput::HandleUnowning(T* obj, BinaryScribe& scribe, TrackerMap& trackers)
    {
        Handle(obj, scribe, trackers, false);
    }

    template<class T>
    PointerOutput::PolymorphicEntry::PolymorphicEntry(const ClassName& className, PointerOutput& owner, Type<T>) :
        id(PointerSpecialID::NULLPTR), className(className), owner(&owner)
    {
        _saveObject = [this](const void* obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns)
        {
            auto casted = static_cast<T*>(RemoveConst(obj));
            this->owner->SaveTracked(casted, scribe, trackers, owns);
            trackers.SignalSerialized(casted);
        };

        _findFromTracker = [this](const void* obj, TrackerMap& trackers) -> TrackingID
        {
            return trackers.FindID(static_cast<T*>(RemoveConst(obj)));
        };

        _track = [this](const void* obj, TrackerMap& trackers) -> TrackingID
        {
            return trackers.Add(static_cast<T*>(RemoveConst(obj)));
            return 0;
        };
    }

    template<class T>
    void PointerOutput::Push(const ClassName& name)
    {
        AddEntry(PolymorphicEntry(name, *this, Type<T>{}), typeid(T));
    }

    template<class T>
    void PointerOutput::Handle(T* obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns)
    {
        if (ShouldHandleNullptr(obj))
            HandleNullptr(scribe);
        else if (ShouldHandleNonpolymorphically(obj))
            HandleNonpolymorphically(obj, scribe, trackers, owns);
        else
            HandlePolymorphically(obj, scribe, trackers, owns);
    }

    template<class T, typename std::enable_if<!std::is_abstract<T>::value, int>::type>
    void PointerOutput::HandleNonpolymorphically(T* obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns)
    {
        auto trackerID = trackers.FindID(obj);
        bool didFindTrackerID = trackerID != invalidTrackingID;
        if (!didFindTrackerID)
            trackerID = trackers.Add(obj);

        scribe.Save(PolymorphicID(PointerSpecialID::HANDLE_NONPOLYMORPHICALLY));
        scribe.Save(trackerID);
        // If just added and owns
        if (owns && !trackers.HasBeenSerialized(obj))
        {
            SaveTracked(obj, scribe, trackers, owns);
            trackers.SignalSerialized(obj);
        }
    }

    template<class T, typename std::enable_if<std::is_abstract<T>::value, int>::type>
    void PointerOutput::HandleNonpolymorphically(T* obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns)
    {}

    template<class T>
    void PointerOutput::HandlePolymorphically(T* obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns)
    {
        // Find the derived type
        auto polymorphicEntry = FindRequiredPolymorphicEntry(obj);

        TrackingID trackerID = invalidTrackingID;
        if (RegisteredTypes::Has<T>())
        {
            trackerID = polymorphicEntry->FindFromTracker(obj, trackers);
            bool didFindTrackerID = trackerID != invalidTrackingID;

            if (!didFindTrackerID)
                trackerID = polymorphicEntry->Track(obj, trackers);
        }

        // Check if should set ID
        bool shouldSaveClassName = false;
        if (polymorphicEntry->id == PointerSpecialID::NULLPTR)
        {
            polymorphicEntry->id = nextID++;
            shouldSaveClassName = true;
        }

        // ---- Actual saving

        // Save polymorphic ID
        polymorphicEntry->SaveID(scribe);
        // Possibly save name
        if (shouldSaveClassName)
            polymorphicEntry->SaveClassName(scribe);

        // Save tracked ID
        scribe.Save(trackerID);
        // If just added and owns
        if (owns && !trackers.HasBeenSerialized(obj))
            polymorphicEntry->SaveObject(obj, scribe, trackers, owns);
    }

    template<class T>
    bool PointerOutput::ShouldHandleNullptr(T* obj)
    {
        return !obj;
    }

    template<class T>
    bool PointerOutput::ShouldHandleNonpolymorphically(T* obj)
    {
        return typeid(*obj) == typeid(T);
    }

    template<class T>
    void PointerOutput::SaveTracked(T* obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns)
    {
        scribe.Save(*obj);
    }

    template<class T>
    std::type_index PointerOutput::TypeOf(T* obj)
    {
        return typeid(*obj);
    }

    template<class T>
    typename PointerOutput::PolymorphicEntry* PointerOutput::FindRequiredPolymorphicEntry(T* obj)
    {
        auto found = types.find(typeid(*obj));
        if (found == types.end())
            throw PolymorphicTypeNotFound();

        return &*found->second;
    }
}