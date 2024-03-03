#pragma once

#include <functional>

#include "ObjectTracker.h"

#include "Scribe.h"
#include "Access.h"

#include "Storage.h"
#include "TypeHandle.h"
#include "Const.h"

#include "TypeHandlesFor.h"
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
        void Scriven(void* object, const std::type_index& type, ArchiveT& archive);
        template<class T>
        void Save(const T* object, ArchiveT& archive);
        void Save(void* object, const std::type_index& type, ArchiveT& archive);
        void Load(void* object, const std::type_index& type, ArchiveT& archive);
        void Construct(void*& storage, const std::type_index& type, ArchiveT& archive);
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
            virtual ~Entry() = 0;

            virtual void Save(const void* object, ArchiveT& archive) = 0;
            virtual void Load(void* object, ArchiveT& archive) = 0;
            virtual void Construct(void*& storage, ArchiveT& archive) = 0;
            [[nodiscard]] virtual void* CreateStorage() const = 0;
        protected:
            Entry
            (
                TypeHandle principleTypeHandle,
                std::vector<TypeHandle> representedTypeHandles,
                std::type_index typeIndex
            );
        };

        template<class T>
        class EntryDerived : public Entry
        {
        public:
            EntryDerived(TypeHandle principleTypeHandle, std::vector<TypeHandle> representedTypeHandles);

            void Save(const void* object, ArchiveT& archive) override;
            void Load(void* object, ArchiveT& archive) override;
            void Construct(void*& storage, ArchiveT& archive) override;
            [[nodiscard]] void* CreateStorage() const override;
        private:
            using DerivedT = typename RemoveConstTrait<T>::type;
            using DerivedScribe = Scribe<DerivedT, ArchiveT>;
        };

        using EntryPtr = std::unique_ptr<Entry>;
        using EntryList = std::vector<EntryPtr>;
        using EntryIterator = typename EntryList::iterator;
        using EntryConstIterator = typename EntryList::const_iterator;
        EntryList entryList;

        using RepresentedTypeHandles = std::vector<TypeHandle>;
        RepresentedTypeHandles representedTypeHandles;

        EntryIterator FindRequiredEntry(const TypeHandle& typeHandle);
        EntryIterator FindRequiredEntry(const std::type_index& typeIndex);
    private:
        template<class T>
        TrackingID RequiredTypeTrackingIDFor(const T* object, ArchiveT& archive);
    };

    template<class Archive>
    void PolymorphicManager<Archive>::Scriven(void* object, const std::type_index& type, ArchiveT& archive)
    {
        if (archive.IsOutput())
            Save(object, type, archive);
        else
            Load(object, type, archive);
    }

    template<class Archive>
    template<class T>
    void PolymorphicManager<Archive>::Save(const T* object, ArchiveT& archive)
    {
        auto typeID = RequiredTypeTrackingIDFor(object, archive);
        auto found = FindRequiredEntry(typeid(*RemoveConst(object)));
        (*found)->Save(object, archive);
    }

    template<class Archive>
    void PolymorphicManager<Archive>::Save(void* object, const std::type_index& type, ArchiveT& archive)
    {
        auto found = FindRequiredEntry(type);
        (*found)->Save(object, archive);
    }

    template<class Archive>
    void PolymorphicManager<Archive>::Load(void* object, const std::type_index& type, ArchiveT& archive)
    {
        auto found = FindRequiredEntry(type);
        (*found)->Load(object, archive);
    }

    template<class Archive>
    void PolymorphicManager<Archive>::Construct(
        void*& storage, const std::type_index& type, ArchiveT& archive)
    {
        auto found = FindRequiredEntry(type);
        (*found)->Construct(storage, archive);
    }

    template<class Archive>
    void* PolymorphicManager<Archive>::CreateStorage(
        const std::type_index& type)
    {
        auto found = FindRequiredEntry(type);
        return (*found)->CreateStorage();
    }

    template<class Archive>
    TypeHandle PolymorphicManager<Archive>::PrincipleTypeHandleFor(const std::type_index& type)
    {
        auto found = FindRequiredEntry(type);
        return (*found)->principleTypeHandle;
    }

    template<class Archive>
    auto PolymorphicManager<Archive>::TypeIndexFor(const TypeHandle& typeHandle) -> std::type_index
    {
        auto found = FindRequiredEntry(typeHandle);
        return (*found)->typeIndex;
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

        auto principleTypeHandle = ::Inscription::PrincipleTypeHandleFor<T>(archive);
        entryList.push_back(std::make_unique<EntryDerived<T>>(principleTypeHandle, newRepresentedTypeHandles));
        std::move
        (
            newRepresentedTypeHandles.begin(),
            newRepresentedTypeHandles.end(),
            std::back_inserter(representedTypeHandles)
        );
    }

    template<class Archive>
    PolymorphicManager<Archive>::Entry::~Entry() = default;

    template<class Archive>
    PolymorphicManager<Archive>::Entry::Entry
    (
        TypeHandle principleTypeHandle,
        std::vector<TypeHandle> representedTypeHandles,
        std::type_index typeIndex
    ) :
        principleTypeHandle(principleTypeHandle), representedTypeHandles(representedTypeHandles), typeIndex(typeIndex)
    {}

    template<class Archive>
    template<class T>
    PolymorphicManager<Archive>::EntryDerived<T>::EntryDerived
    (
        TypeHandle principleTypeHandle,
        std::vector<TypeHandle> representedTypeHandles
    ) :
        Entry(principleTypeHandle, representedTypeHandles, typeid(T))
    {}

    template<class Archive>
    template<class T>
    void PolymorphicManager<Archive>::EntryDerived<T>::Save(const void* object, ArchiveT& archive)
    {
        auto castedObject = reinterpret_cast<const T*>(object);
        DerivedScribe derivedScribe;
        derivedScribe.Scriven(RemoveConst(*castedObject), archive);
    }

    template<class Archive>
    template<class T>
    void PolymorphicManager<Archive>::EntryDerived<T>::Load(void* object, ArchiveT& archive)
    {
        auto castedObject = reinterpret_cast<T*>(object);
        DerivedScribe derivedScribe;
        derivedScribe.Scriven(RemoveConst(*castedObject), archive);
    }

    template<class Archive>
    template<class T>
    void PolymorphicManager<Archive>::EntryDerived<T>::Construct(void*& storage, ArchiveT& archive)
    {
        DerivedT* castedStorage = reinterpret_cast<DerivedT*>(storage);
        DerivedScribe derivedScribe;
        Access::Construct(castedStorage, archive, derivedScribe);
    }

    template<class Archive>
    template<class T>
    void* PolymorphicManager<Archive>::EntryDerived<T>::CreateStorage() const
    {
        return ::Inscription::CreateStorage(sizeof(T));
    }

    template<class Archive>
    typename PolymorphicManager<Archive>::EntryIterator PolymorphicManager<Archive>::FindRequiredEntry(
        const TypeHandle& typeHandle)
    {
        for (auto loop = entryList.begin(); loop != entryList.end(); ++loop)
        {
            auto checkBegin = (*loop)->representedTypeHandles.begin();
            auto checkEnd = (*loop)->representedTypeHandles.end();
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
            if ((*loop)->typeIndex == typeIndex)
                return loop;

        throw RepresentedTypeHandleNotFound(typeIndex);
    }

    template<class Archive>
    template<class T>
    TrackingID PolymorphicManager<Archive>::RequiredTypeTrackingIDFor(const T* object, ArchiveT& archive)
    {
        auto& type = typeid(*RemoveConst(object));
        auto found = archive.typeTracker.FindID(type);
        if (found.has_value())
            return *found;

        throw RepresentedTypeHandleNotFound(type);
    }
}