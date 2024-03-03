#pragma once

#include <functional>

#include "ObjectTracker.h"

#include "Scribe.h"
#include "Access.h"

#include "Storage.h"
#include "Type.h"
#include "Const.h"

#include "TypesFor.h"
#include "InputTypeNotFound.h"
#include "InputTypesAlreadyRegistered.h"

namespace Inscription
{
    template<class Archive>
    class PolymorphicManager
    {
    public:
        using ArchiveT = Archive;
    public:
        void Scriven(void* object, const std::type_index& typeIndex, ArchiveT& archive);
        template<class T>
        void Save(const T* object, ArchiveT& archive);
        void Save(void* object, const std::type_index& typeIndex, ArchiveT& archive);
        void Load(void* object, const std::type_index& typeIndex, ArchiveT& archive);
        void Construct(void*& storage, const std::type_index& typeIndex, ArchiveT& archive);
        void* CreateStorage(const std::type_index& typeIndex);

        Type OutputTypeFor(const std::type_index& typeIndex);
        std::type_index TypeIndexFor(const Type& type);

        template<class T>
        void Register(ArchiveT& archive);
    private:
        class Entry
        {
        public:
            const Type outputType;
            const std::vector<Type> inputTypes;
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
                Type outputType,
                std::vector<Type> inputTypes,
                std::type_index typeIndex
            );
        };

        template<class T>
        class EntryDerived : public Entry
        {
        public:
            EntryDerived(Type outputType, std::vector<Type> inputTypes);

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

        using InputTypes = std::vector<Type>;
        InputTypes inputTypes;

        EntryIterator FindRequiredEntry(const Type& type);
        EntryIterator FindRequiredEntry(const std::type_index& typeIndex);
    private:
        template<class T>
        TrackingID RequiredTypeTrackingIDFor(const T* object, ArchiveT& archive);
    };

    template<class Archive>
    void PolymorphicManager<Archive>::Scriven(void* object, const std::type_index& typeIndex, ArchiveT& archive)
    {
        if (archive.IsOutput())
            Save(object, typeIndex, archive);
        else
            Load(object, typeIndex, archive);
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
    void PolymorphicManager<Archive>::Save(void* object, const std::type_index& typeIndex, ArchiveT& archive)
    {
        auto found = FindRequiredEntry(typeIndex);
        (*found)->Save(object, archive);
    }

    template<class Archive>
    void PolymorphicManager<Archive>::Load(void* object, const std::type_index& typeIndex, ArchiveT& archive)
    {
        auto found = FindRequiredEntry(typeIndex);
        (*found)->Load(object, archive);
    }

    template<class Archive>
    void PolymorphicManager<Archive>::Construct(
        void*& storage, const std::type_index& typeIndex, ArchiveT& archive)
    {
        auto found = FindRequiredEntry(typeIndex);
        (*found)->Construct(storage, archive);
    }

    template<class Archive>
    void* PolymorphicManager<Archive>::CreateStorage(
        const std::type_index& typeIndex)
    {
        auto found = FindRequiredEntry(typeIndex);
        return (*found)->CreateStorage();
    }

    template<class Archive>
    Type PolymorphicManager<Archive>::OutputTypeFor(const std::type_index& typeIndex)
    {
        auto found = FindRequiredEntry(typeIndex);
        return (*found)->outputType;
    }

    template<class Archive>
    auto PolymorphicManager<Archive>::TypeIndexFor(const Type& type) -> std::type_index
    {
        auto found = FindRequiredEntry(type);
        return (*found)->typeIndex;
    }

    template<class Archive>
    template<class T>
    void PolymorphicManager<Archive>::Register(ArchiveT& archive)
    {
        auto newInputTypes = InputTypesFor<T>(archive);

        std::vector<Type> duplicateTypes;
        for (auto& currentInput : inputTypes)
            for (auto& currentNewInput : newInputTypes)
                if (currentNewInput == currentInput)
                    duplicateTypes.push_back(currentNewInput);

        if (!duplicateTypes.empty())
            throw InputTypesAlreadyRegistered(duplicateTypes);

        auto outputType = ::Inscription::OutputTypeFor<T>(archive);
        entryList.push_back(std::make_unique<EntryDerived<T>>(outputType, newInputTypes));
        std::move
        (
            newInputTypes.begin(),
            newInputTypes.end(),
            std::back_inserter(inputTypes)
        );
    }

    template<class Archive>
    PolymorphicManager<Archive>::Entry::~Entry() = default;

    template<class Archive>
    PolymorphicManager<Archive>::Entry::Entry
    (
        Type outputType,
        std::vector<Type> inputTypes,
        std::type_index typeIndex
    ) :
        outputType(outputType), inputTypes(inputTypes), typeIndex(typeIndex)
    {}

    template<class Archive>
    template<class T>
    PolymorphicManager<Archive>::EntryDerived<T>::EntryDerived
    (
        Type outputType,
        std::vector<Type> inputTypes
    ) :
        Entry(outputType, inputTypes, typeid(T))
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
        const Type& type)
    {
        for (auto loop = entryList.begin(); loop != entryList.end(); ++loop)
        {
            auto checkBegin = (*loop)->inputTypes.begin();
            auto checkEnd = (*loop)->inputTypes.end();
            if (std::find(checkBegin, checkEnd, type) != checkEnd)
                return loop;
        }

        throw InputTypeNotFound(type);
    }

    template<class Archive>
    typename PolymorphicManager<Archive>::EntryIterator PolymorphicManager<Archive>::FindRequiredEntry(
        const std::type_index& typeIndex)
    {
        for (auto loop = entryList.begin(); loop != entryList.end(); ++loop)
            if ((*loop)->typeIndex == typeIndex)
                return loop;

        throw InputTypeNotFound(typeIndex);
    }

    template<class Archive>
    template<class T>
    TrackingID PolymorphicManager<Archive>::RequiredTypeTrackingIDFor(const T* object, ArchiveT& archive)
    {
        auto& type = typeid(*RemoveConst(object));
        auto found = archive.types.FindTypeID(type);
        if (found.has_value())
            return *found;

        throw InputTypeNotFound(type);
    }
}