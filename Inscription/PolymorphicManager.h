#pragma once

#include <functional>

#include "ObjectTracker.h"
#include "TypeTracker.h"

#include "ClassName.h"

#include "Type.h"

#include "PolymorphicTypeNotFound.h"

namespace Inscription
{
    template<class Archive>
    class PolymorphicManager
    {
    public:
        using ArchiveT = Archive;
    public:
        template<class T>
        void SaveConstruction(const T* object, ArchiveT& archive);
        template<class T>
        void ConstructFromLoad(T*& object, const ClassName& className, ArchiveT& archive);

        ClassName ClassNameFor(TrackingID type, ArchiveT& archive);

        template<class T>
        void Register(ArchiveT& archive);
    private:
        class Entry
        {
        public:
            const ClassName className;
            const TrackingID typeID;
        public:
            template<class T>
            Entry(const ClassName& className, TypeTracker& typeTracker, Type<T>);

            void SaveConstruction(const void* object, ArchiveT& archive);
            void ConstructFromLoad(void*& object, ArchiveT& archive);
        private:
            std::function<void(const void*, ArchiveT&)> _saveConstruction;
            std::function<void(void*&, ArchiveT&)> _constructFromLoad;
        };

        typedef std::vector<Entry> EntryList;
        typedef typename EntryList::iterator EntryIterator;
        typedef typename EntryList::const_iterator EntryConstIterator;
        EntryList entryList;

        EntryIterator FindRequiredEntry(const ClassName& className);
        EntryIterator FindRequiredEntry(TrackingID typeID);
        EntryIterator FindRequiredEntry(TrackingID typeID, const std::type_index& type);
    private:
        template<class T>
        TrackingID RequiredTypeIDFor(const T* object, ArchiveT& archive);
    };

    template<class Archive>
    template<class T>
    void PolymorphicManager<Archive>::SaveConstruction(const T* object, ArchiveT& archive)
    {
        auto typeID = RequiredTypeIDFor(object, archive);
        auto found = FindRequiredEntry(typeID, typeid(*RemoveConst(object)));
        found->SaveConstruction(object, archive);
    }

    template<class Archive>
    template<class T>
    void PolymorphicManager<Archive>::ConstructFromLoad(T*& object, const ClassName& className, ArchiveT& archive)
    {
        auto found = FindRequiredEntry(className);
        found->ConstructFromLoad(object, archive);
    }

    template<class Archive>
    ClassName PolymorphicManager<Archive>::ClassNameFor(TrackingID type, ArchiveT& archive)
    {
        auto found = FindRequiredEntry(type);
        return found->className;
    }

    template<class Archive>
    template<class T>
    void PolymorphicManager<Archive>::Register(ArchiveT& archive)
    {
        auto className = Scribe<T, ArchiveT>::classNameResolver.NameFor(archive);
        entryList.push_back(Entry(className, archive.typeTracker, Type<T>{}));
    }

    template<class Archive>
    template<class T>
    PolymorphicManager<Archive>::Entry::Entry(const ClassName& className, TypeTracker& typeTracker, Type<T>) :
        className(className), typeID(typeTracker.Add<T>())
    {
        using DerivedScribe = Scribe<typename RemoveConstTrait<T>::type, ArchiveT>;

        _saveConstruction = [](const void* object, ArchiveT& archive)
        {
            auto castedObject = static_cast<const T*>(object);
            DerivedScribe::Scriven(RemoveConst(*castedObject), archive);
        };

        _constructFromLoad = [](void*& object, ArchiveT& archive)
        {
            T* storage = nullptr;
            DerivedScribe::Construct(storage, archive);
            object = storage;
        };
    }

    template<class Archive>
    void PolymorphicManager<Archive>::Entry::SaveConstruction(const void* object, ArchiveT& archive)
    {
        _saveConstruction(object, archive);
    }

    template<class Archive>
    void PolymorphicManager<Archive>::Entry::ConstructFromLoad(void*& object, ArchiveT& archive)
    {
        _constructFromLoad(object, archive);
    }

    template<class Archive>
    typename PolymorphicManager<Archive>::EntryIterator PolymorphicManager<Archive>::FindRequiredEntry(const ClassName& className)
    {
        for (auto loop = entryList.begin(); loop != entryList.end(); ++loop)
            if (loop->className == className)
                return loop;

        throw PolymorphicTypeNotFound(className);
    }

    template<class Archive>
    typename PolymorphicManager<Archive>::EntryIterator PolymorphicManager<Archive>::FindRequiredEntry(TrackingID typeID)
    {
        for (auto loop = entryList.begin(); loop != entryList.end(); ++loop)
            if (loop->typeID == typeID)
                return loop;

        throw PolymorphicTypeNotFound();
    }

    template<class Archive>
    typename PolymorphicManager<Archive>::EntryIterator PolymorphicManager<Archive>::FindRequiredEntry(TrackingID typeID, const std::type_index& type)
    {
        for (auto loop = entryList.begin(); loop != entryList.end(); ++loop)
            if (loop->typeID == typeID)
                return loop;

        throw PolymorphicTypeNotFound(type);
    }

    template<class Archive>
    template<class T>
    TrackingID PolymorphicManager<Archive>::RequiredTypeIDFor(const T* object, ArchiveT& archive)
    {
        auto& type = typeid(*RemoveConst(object));
        auto found = archive.typeTracker.FindID(type);
        if (found.IsValid())
            return *found;

        throw PolymorphicTypeNotFound(type);
    }
}