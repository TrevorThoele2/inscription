#pragma once

#include "Polymorphic.h"

namespace Inscription
{
    template<class T>
    void PolymorphicManagerRegistration::Register()
    {
        Derived<T>::instance;
    }

    template<class T>
    typename PolymorphicManagerRegistration::Derived<T> PolymorphicManagerRegistration::Derived<T>::instance;

    template<class T>
    void PolymorphicManager::HandleObject(T *&obj, Scribe &scribe, TrackerGroup &trackers)
    {
        switch (direction)
        {
        case Direction::OUTPUT:
            static_cast<OutputImpl*>(implementation.get())->HandleObject(obj, scribe, trackers);
            break;
        case Direction::INPUT:
            static_cast<InputImpl*>(implementation.get())->HandleObject(obj, scribe, trackers);
            break;
        }
    }

    template<class T>
    void PolymorphicManager::HandleObjectNoConstruct(T *&obj, Scribe &scribe, TrackerGroup &trackers)
    {
        switch (direction)
        {
        case Direction::OUTPUT:
            static_cast<OutputImpl*>(implementation.get())->HandleObjectNoConstruct(obj, scribe, trackers);
            break;
        case Direction::INPUT:
            static_cast<InputImpl*>(implementation.get())->HandleObjectNoConstruct(obj, scribe, trackers);
            break;
        }
    }

    template<class T>
    void PolymorphicManager::Register()
    {
        PolymorphicManagerRegistration::Register<T>();
    }

    template<class T>
    void PolymorphicManager::OutputImpl::HandleObject(T *&obj, Scribe &scribe, TrackerGroup &trackers)
    {
        // If object is null, save null ID
        if (!obj)
        {
            scribe.WriteNumericImplementation(PolymorphicID(SpecialIDs::NULLPTR));
            return;
        }

        // If object doesn't need polymorphic handling...
        if (typeid(*obj) == typeid(T))
        {
            // Save ID for "no polymorphic handling needed"
            HandleObjectSpecial(scribe, trackers, obj);
            return;
        }

        auto found = types.find(typeid(*obj));
        if (found == types.end())
            throw PolymorphicTypeNotFoundException();

        auto entry = found->second->get();
        // Enable tracking (code auto-generates on startup, so this can be called anywhere)
        entry->EnableTracking(trackers);

        // Do actual saving
        auto &tracked = trackers.Add(obj, typeid(*obj), std::integral_constant<bool, TrackPointer<typename std::remove_cv<T>::type>::value != TRACK_NEVER>{});
        auto justAdded = tracked.first;
        auto trackingEntry = tracked.second;

        // Check if should set ID
        bool saveName = false;
        if (entry->id == SpecialIDs::NULLPTR)
        {
            entry->id = nextID++;
            saveName = true;
        }

        // Save polymorphic ID
        entry->SaveID(scribe);
        // Possibly save name
        if (saveName)
            scribe.Save(RemoveConst(entry->GetName()));

        // Save tracked ID
        scribe.WriteNumericImplementation(trackingEntry->GetID());
        // If the tracker just added this object
        if (justAdded)
            (*found->second)->SaveObject(obj, scribe); // Save the object iself afterward
    }

    template<class T>
    void PolymorphicManager::OutputImpl::HandleObjectNoConstruct(T *&obj, Scribe &scribe, TrackerGroup &trackers)
    {
        // If object is null, save null ID
        if (!obj)
        {
            scribe.WriteNumericImplementation(PolymorphicID(SpecialIDs::NULLPTR));
            return;
        }

        if (typeid(*obj) == typeid(T))
        {
            auto &tracked = trackers.Add(obj);
            // Save ID for "no polymorphic handling needed"
            scribe.WriteNumericImplementation(PolymorphicID(SpecialIDs::HANDLE_NORMALLY));
            scribe.WriteNumericImplementation(tracked.second->GetID());
            return;
        }

        // Find the real type
        auto found = types.find(typeid(*obj));
        if (found == types.end())
            throw PolymorphicTypeNotFoundException();

        auto entry = found->second->get();
        // Enable tracking (code auto-generates on startup, so this can be called anywhere)
        entry->EnableTracking(trackers);

        // Do actual saving
        auto &tracked = trackers.Add(obj, typeid(*obj), std::integral_constant<bool, TrackPointer<typename std::remove_cv<T>::type>::value != TRACK_NEVER>{});
        auto trackingEntry = tracked.second;

        // Check if should set ID
        bool saveName = false;
        if (entry->id == SpecialIDs::NULLPTR)
        {
            entry->id = nextID++;
            saveName = true;
        }

        // Save polymorphic ID
        entry->SaveID(scribe);
        // Possibly save name
        if (saveName)
            scribe.Save(RemoveConst(entry->GetName()));

        // Save tracked ID
        scribe.WriteNumericImplementation(trackingEntry->GetID());
    }

    template<class T>
    void PolymorphicManager::OutputImpl::Push(const ClassName &name)
    {
        list.push_back(EntryPtr(new Entry<T>(name)));
        types.emplace(typeid(T), --list.end());
    }

    template<class T>
    PolymorphicManager::OutputImpl::Entry<T>::Entry(const ClassName &name) : EntryBase(name)
    {}

    template<class T>
    void PolymorphicManager::OutputImpl::Entry<T>::SaveObject(const void *obj, Scribe &scribe)
    {
        scribe.Save(*static_cast<T*>(RemoveConst(obj)));
    }

    template<class T>
    void PolymorphicManager::OutputImpl::Entry<T>::EnableTracking(TrackerGroup &trackers)
    {
        trackers.AttemptEnableSelectiveTracking<T>();
        trackers.AttemptEnableAlwaysTracking<T>();
    }

    template<class T, typename std::enable_if<!std::is_abstract<T>::value, int>::type>
    void PolymorphicManager::OutputImpl::HandleObjectSpecial(Scribe &scribe, TrackerGroup &trackers, T *obj)
    {
        auto &tracked = trackers.Add(obj);

        scribe.WriteNumericImplementation(PolymorphicID(SpecialIDs::HANDLE_NORMALLY));
        scribe.WriteNumericImplementation(tracked.second->GetID());
        // If just added
        if (tracked.first)
            scribe.Save(*obj);
    }

    template<class T>
    void PolymorphicManager::InputImpl::HandleObject(T *&obj, Scribe &scribe, TrackerGroup &trackers)
    {
        // Do actual loading
        PolymorphicID id = SpecialIDs::NULLPTR;
        // Load polymorphic ID
        scribe.ReadNumeric(id);
        // If ID is the nullptr ID
        if (id == SpecialIDs::NULLPTR)
        {
            obj = nullptr;
            return;
        }

        // If ID indicates doesn't need polymorphic handling
        if (HandleObjectSpecial(scribe, id, obj))
            return;

        ClassName *name = nullptr;
        // Find the name
        {
            auto foundId = ids.find(id);
            if (foundId == ids.end())
            {
                ClassName loadName;
                scribe.Load(loadName);

                auto foundName = classNames.find(loadName);
                if (foundName == classNames.end())
                    throw PolymorphicTypeNotFoundException();

                name = &ids.emplace(nextID++, loadName).first->second;
            }
            else
                name = &foundId->second;
        }

        auto entry = classNames.find(*name)->second->get();
        entry->EnableTracking(trackers);
        // Loads the tracker ID
        obj = static_cast<T*>(entry->GetObject(scribe));
    }

    template<class T>
    void PolymorphicManager::InputImpl::HandleObjectNoConstruct(T *&obj, Scribe &scribe, TrackerGroup &trackers)
    {
        // Do actual loading
        PolymorphicID id = SpecialIDs::NULLPTR;
        // Load polymorphic ID
        scribe.ReadNumeric(id);
        // If ID is the nullptr ID
        if (id == SpecialIDs::NULLPTR)
        {
            obj = nullptr;
            return;
        }

        // Handle special potentially
        if (id == SpecialIDs::HANDLE_NORMALLY)
        {
            scribe.SimpleLoadTracked(obj);
            return;
        }

        ClassName *name = nullptr;
        // Find the name
        {
            auto foundId = ids.find(id);
            if (foundId == ids.end())
            {
                ClassName loadName;
                scribe.Load(loadName);

                auto foundName = classNames.find(loadName);
                if (foundName == classNames.end())
                    throw PolymorphicTypeNotFoundException();

                name = &ids.emplace(nextID++, loadName).first->second;
            }
            else
                name = &foundId->second;
        }

        auto entry = classNames.find(*name)->second->get();
        entry->EnableTracking(trackers);
        // Loads the tracker ID
        obj = static_cast<T*>(entry->GetObjectNoTrack(scribe));
    }

    template<class T>
    void PolymorphicManager::InputImpl::Push(const ClassName &name)
    {
        list.push_back(EntryPtr(new Entry<T>(name)));
        classNames.emplace(name, --list.end());
    }

    template<class T>
    PolymorphicManager::InputImpl::Entry<T>::Entry(const ClassName &name) : EntryBase(name)
    {}

    template<class T>
    void* PolymorphicManager::InputImpl::Entry<T>::GetObject(Scribe &scribe)
    {
        T *var = nullptr;
        scribe.LoadTracked(var);
        return var;
    }

    template<class T>
    void* PolymorphicManager::InputImpl::Entry<T>::GetObjectNoTrack(Scribe &scribe)
    {
        T *var = nullptr;
        scribe.SimpleLoadTracked(var);
        return var;
    }

    template<class T>
    void PolymorphicManager::InputImpl::Entry<T>::EnableTracking(TrackerGroup &trackers)
    {
        trackers.AttemptEnableSelectiveTracking<T>();
        trackers.AttemptEnableAlwaysTracking<T>();
    }

    template<class T, typename std::enable_if<!std::is_abstract<T>::value, int>::type>
    bool PolymorphicManager::InputImpl::HandleObjectSpecial(Scribe &scribe, PolymorphicID id, T *&obj)
    {
        if (id == SpecialIDs::HANDLE_NORMALLY)
        {
            scribe.LoadTracked(obj);
            return true;
        }

        return false;
    }

    template<class T>
    void PolymorphicManager::Add(const ClassName &name)
    {
        switch (direction)
        {
        case Direction::OUTPUT:
            static_cast<OutputImpl*>(implementation.get())->Push<T>(name);
            break;
        case Direction::INPUT:
            static_cast<InputImpl*>(implementation.get())->Push<T>(name);
            break;
        }
    }

    template<class T>
    PolymorphicManagerRegistration::Derived<T>::Derived()
    {
        PolymorphicManagerRegistration::Instance().types.push_back(this);
    }

    template<class T>
    void PolymorphicManagerRegistration::Derived<T>::Fill(PolymorphicManager &manager, Scribe &scribe)
    {
        manager.Add<T>(Access::InscripterT<T>::GetClassNameContainer().GetName(scribe));
    }
}