#pragma once

#include <functional>
#include <algorithm>

#include "ObjectTracker.h"

#include "Scribe.h"
#include "Access.h"

#include "Storage.h"
#include "Type.h"
#include "TypeHandle.h"
#include "Const.h"

#include "ScribeHasRepresentedTypeHandles.h"

#include "RepresentedTypeHandleNotFound.h"
#include "RepresentedTypeHandlesAlreadyRegistered.h"

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
        void ConstructFromLoad(void*& storage, const std::type_index& type, ArchiveT& archive);
        void* CreateStorage(const std::type_index& type);

        TypeHandle PrincipleTypeHandleFor(const std::type_index& type);
        std::type_index TypeIndexFor(const TypeHandle& typeHandle);

        template<class T>
        void Register(ArchiveT& archive);
    private:
        class Entry
        {
        public:
            const TypeHandle principleTypeHandle;
            const std::vector<TypeHandle> representedTypeHandles;
            const std::type_index typeIndex;
        public:
            template<class T>
            Entry(TypeHandle principleTypeHandle, std::vector<TypeHandle> representedTypeHandles, Type<T>);

            void SaveConstruction(const void* object, ArchiveT& archive);
            void ConstructFromLoad(void*& storage, ArchiveT& archive);
            [[nodiscard]] void* CreateStorage() const;
        private:
            std::function<void(const void*, ArchiveT&)> _saveConstruction;
            std::function<void(void*&, ArchiveT&)> _constructFromLoad;
            std::function<void*()> _createStorage;
        };

        using EntryList = std::vector<Entry>;
        using EntryIterator = typename EntryList::iterator;
        using EntryConstIterator = typename EntryList::const_iterator;
        EntryList entryList;

        using RepresentedTypeHandles = std::vector<TypeHandle>;
        RepresentedTypeHandles representedTypeHandles;

        EntryIterator FindRequiredEntry(const TypeHandle& typeHandle);
        EntryIterator FindRequiredEntry(const std::type_index& typeIndex);
    private:
        template<class T>
        static TypeHandle PrincipleTypeHandleFor(Archive& archive)
        {
            using RegisteredScribe = Scribe<T, ArchiveT>;
            return RegisteredScribe::PrincipleTypeHandle(archive);
        }

        template<class T, std::enable_if_t<scribe_has_represented_type_handles_v<T, Archive>, int> = 0>
        static std::vector<TypeHandle> RepresentedTypeHandlesFor(Archive& archive)
        {
            using RegisteredScribe = Scribe<T, ArchiveT>;
            auto principleTypeHandle = RegisteredScribe::PrincipleTypeHandle(archive);
            auto representedTypeHandles = RegisteredScribe::RepresentedTypeHandles(archive);

            {
                std::vector<TypeHandle> duplicateTypeHandles;
                for (auto& currentRepresented : representedTypeHandles)
                {
                    auto count = std::count(
                        representedTypeHandles.begin(),
                        representedTypeHandles.end(),
                        currentRepresented);
                    if (count > 1)
                        duplicateTypeHandles.push_back(currentRepresented);
                }

                if (!duplicateTypeHandles.empty())
                    throw RepresentedTypeHandlesAlreadyRegistered(duplicateTypeHandles);
            }

            auto isPrincipleIn = [principleTypeHandle](const TypeHandle& typeHandle)
            {
                return typeHandle == principleTypeHandle;
            };

            if (!std::any_of(representedTypeHandles.begin(), representedTypeHandles.end(), isPrincipleIn))
                representedTypeHandles.push_back(principleTypeHandle);

            return representedTypeHandles;
        }

        template<class T, std::enable_if_t<!scribe_has_represented_type_handles_v<T, Archive>, int> = 0>
        static std::vector<TypeHandle> RepresentedTypeHandlesFor(Archive& archive)
        {
            using RegisteredScribe = Scribe<T, ArchiveT>;
            return std::vector<TypeHandle> { RegisteredScribe::PrincipleTypeHandle(archive) };
        }
    private:
        template<class T>
        TrackingID RequiredTypeTrackingIDFor(const T* object, ArchiveT& archive);
    };

    template<class Archive>
    template<class T>
    void PolymorphicManager<Archive>::SaveConstruction(const T* object, ArchiveT& archive)
    {
        auto typeID = RequiredTypeTrackingIDFor(object, archive);
        auto found = FindRequiredEntry(typeid(*RemoveConst(object)));
        found->SaveConstruction(object, archive);
    }

    template<class Archive>
    void PolymorphicManager<Archive>::ConstructFromLoad(
        void*& storage, const std::type_index& type, ArchiveT& archive)
    {
        auto found = FindRequiredEntry(type);
        found->ConstructFromLoad(storage, archive);
    }

    template<class Archive>
    void* PolymorphicManager<Archive>::CreateStorage(
        const std::type_index& type)
    {
        auto found = FindRequiredEntry(type);
        return found->CreateStorage();
    }

    template<class Archive>
    TypeHandle PolymorphicManager<Archive>::PrincipleTypeHandleFor(const std::type_index& type)
    {
        auto found = FindRequiredEntry(type);
        return found->principleTypeHandle;
    }

    template<class Archive>
    auto PolymorphicManager<Archive>::TypeIndexFor(const TypeHandle& typeHandle) -> std::type_index
    {
        auto found = FindRequiredEntry(typeHandle);
        return found->typeIndex;
    }

    template<class Archive>
    template<class T>
    void PolymorphicManager<Archive>::Register(ArchiveT& archive)
    {
        auto newRepresentedTypeHandles = RepresentedTypeHandlesFor<T>(archive);

        std::vector<TypeHandle> duplicateTypeHandles;
        for (auto& currentRepresented : representedTypeHandles)
            for (auto& currentNewRepresented : newRepresentedTypeHandles)
                if (currentNewRepresented == currentRepresented)
                    duplicateTypeHandles.push_back(currentNewRepresented);

        if (!duplicateTypeHandles.empty())
            throw RepresentedTypeHandlesAlreadyRegistered(duplicateTypeHandles);

        auto principleTypeHandle = PrincipleTypeHandleFor<T>(archive);
        entryList.push_back(Entry(principleTypeHandle, newRepresentedTypeHandles, ::Inscription::Type<T>{}));
        std::move
        (
            newRepresentedTypeHandles.begin(),
            newRepresentedTypeHandles.end(),
            std::back_inserter(representedTypeHandles)
        );
    }

    template<class Archive>
    template<class T>
    PolymorphicManager<Archive>::Entry::Entry(
        TypeHandle principleTypeHandle,
        std::vector<TypeHandle> representedTypeHandles,
        Type<T>)
        :
        principleTypeHandle(std::move(principleTypeHandle)),
        representedTypeHandles(std::move(representedTypeHandles)),
        typeIndex(typeid(T))
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
            Access::Construct(castedStorage, archive, derivedScribe);
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
    void* PolymorphicManager<Archive>::Entry::CreateStorage() const
    {
        return _createStorage();
    }

    template<class Archive>
    typename PolymorphicManager<Archive>::EntryIterator PolymorphicManager<Archive>::FindRequiredEntry(
        const TypeHandle& typeHandle)
    {
        for (auto loop = entryList.begin(); loop != entryList.end(); ++loop)
        {
            auto checkBegin = loop->representedTypeHandles.begin();
            auto checkEnd = loop->representedTypeHandles.end();
            if (std::find(checkBegin, checkEnd, typeHandle) != checkEnd)
                return loop;
        }

        throw RepresentedTypeHandleNotFound(typeHandle);
    }

    template<class Archive>
    typename PolymorphicManager<Archive>::EntryIterator PolymorphicManager<Archive>::FindRequiredEntry(
        const std::type_index& typeIndex)
    {
        for (auto loop = entryList.begin(); loop != entryList.end(); ++loop)
            if (loop->typeIndex == typeIndex)
                return loop;

        throw RepresentedTypeHandleNotFound(typeIndex);
    }

    template<class Archive>
    template<class T>
    TrackingID PolymorphicManager<Archive>::RequiredTypeTrackingIDFor(const T* object, ArchiveT& archive)
    {
        auto& type = typeid(*RemoveConst(object));
        auto found = archive.typeTracker.FindID(type);
        if (found.IsValid())
            return *found;

        throw RepresentedTypeHandleNotFound(type);
    }
}