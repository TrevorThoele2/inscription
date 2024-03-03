#pragma once

#include <functional>

#include "ObjectTracker.h"
#include "TypeTracker.h"

#include "Scribe.h"

#include "Storage.h"
#include "ClassName.h"
#include "Type.h"
#include "Const.h"

#include "PolymorphicTypeNotFound.h"

namespace Inscription
{
    template<class Archive>
    class PolymorphicManager
    {
    public:
        using ArchiveT = Archive;

        using Type = std::type_index;
    public:
        template<class T>
        void SaveConstruction(const T* object, ArchiveT& archive);
        void ConstructFromLoad(void*& storage, const Type& type, ArchiveT& archive);
        void* CreateStorage(const Type& type);

        ClassName ClassNameFor(const Type& type);
        Type TypeFor(const ClassName& className);

        template<class T>
        void Register(ArchiveT& archive);
    private:
        class Entry
        {
        public:
            const ClassName className;
            const Type type;
        public:
            template<class T>
            Entry(const ClassName& className, ::Inscription::Type<T>);

            void SaveConstruction(const void* object, ArchiveT& archive);
            void ConstructFromLoad(void*& storage, ArchiveT& archive);
            void* CreateStorage();
        private:
            std::function<void(const void*, ArchiveT&)> _saveConstruction;
            std::function<void(void*&, ArchiveT&)> _constructFromLoad;
            std::function<void*()> _createStorage;
        };

        using EntryList = std::vector<Entry>;
        using EntryIterator = typename EntryList::iterator;
        using EntryConstIterator = typename EntryList::const_iterator;
        EntryList entryList;

        EntryIterator FindRequiredEntry(const ClassName& className);
        EntryIterator FindRequiredEntry(const Type& type);
    private:
        template<class T>
        TrackingID RequiredTypeIDFor(const T* object, ArchiveT& archive);
    };

    template<class Archive>
    template<class T>
    void PolymorphicManager<Archive>::SaveConstruction(const T* object, ArchiveT& archive)
    {
        auto typeID = RequiredTypeIDFor(object, archive);
        auto found = FindRequiredEntry(typeid(*RemoveConst(object)));
        found->SaveConstruction(object, archive);
    }

    template<class Archive>
    void PolymorphicManager<Archive>::ConstructFromLoad(
        void*& storage, const Type& type, ArchiveT& archive)
    {
        auto found = FindRequiredEntry(type);
        found->ConstructFromLoad(storage, archive);
    }

    template<class Archive>
    void* PolymorphicManager<Archive>::CreateStorage(
        const Type& type)
    {
        auto found = FindRequiredEntry(type);
        return found->CreateStorage();
    }

    template<class Archive>
    ClassName PolymorphicManager<Archive>::ClassNameFor(const Type& type)
    {
        auto found = FindRequiredEntry(type);
        return found->className;
    }

    template<class Archive>
    auto PolymorphicManager<Archive>::TypeFor(const ClassName& className) -> Type
    {
        auto found = FindRequiredEntry(className);
        return found->type;
    }

    template<class Archive>
    template<class T>
    void PolymorphicManager<Archive>::Register(ArchiveT& archive)
    {
        auto className = Scribe<T, ArchiveT>::classNameResolver.NameFor(archive);
        entryList.push_back(Entry(className, ::Inscription::Type<T>{}));
    }

    template<class Archive>
    template<class T>
    PolymorphicManager<Archive>::Entry::Entry(const ClassName& className, ::Inscription::Type<T>) :
        className(className), type(typeid(T))
    {
        using DerivedT = typename RemoveConstTrait<T>::type;
        using DerivedScribe = Scribe<DerivedT, ArchiveT>;

        _saveConstruction = [](const void* object, ArchiveT& archive)
        {
            auto castedObject = reinterpret_cast<const T*>(object);
            DerivedScribe derivedScribe;
            derivedScribe.Scriven(RemoveConst(*castedObject), archive);
        };

        _constructFromLoad = [](void*& storage, ArchiveT& archive)
        {
            DerivedT* castedStorage = reinterpret_cast<DerivedT*>(storage);
            DerivedScribe derivedScribe;
            derivedScribe.Construct(castedStorage, archive);
        };

        _createStorage = []() -> void*
        {
            return ::Inscription::CreateStorage(sizeof(T));
        };
    }

    template<class Archive>
    void PolymorphicManager<Archive>::Entry::SaveConstruction(const void* object, ArchiveT& archive)
    {
        _saveConstruction(object, archive);
    }

    template<class Archive>
    void PolymorphicManager<Archive>::Entry::ConstructFromLoad(void*& storage, ArchiveT& archive)
    {
        return _constructFromLoad(storage, archive);
    }

    template<class Archive>
    void* PolymorphicManager<Archive>::Entry::CreateStorage()
    {
        return _createStorage();
    }

    template<class Archive>
    typename PolymorphicManager<Archive>::EntryIterator PolymorphicManager<Archive>::FindRequiredEntry(
        const ClassName& className)
    {
        for (auto loop = entryList.begin(); loop != entryList.end(); ++loop)
            if (loop->className == className)
                return loop;

        throw PolymorphicTypeNotFound(className);
    }

    template<class Archive>
    typename PolymorphicManager<Archive>::EntryIterator PolymorphicManager<Archive>::FindRequiredEntry(
        const Type& type)
    {
        for (auto loop = entryList.begin(); loop != entryList.end(); ++loop)
            if (loop->type == type)
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