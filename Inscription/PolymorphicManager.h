#pragma once

#include <functional>
#include <iterator>

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
    template<class Format>
    class PolymorphicManager
    {
    public:
        using FormatT = Format;
    public:
        void Scriven(void* object, const std::type_index& typeIndex, FormatT& format);
        template<class T>
        void Save(const T* object, FormatT& format);
        void Save(void* object, const std::type_index& typeIndex, FormatT& format);
        void Load(void* object, const std::type_index& typeIndex, FormatT& format);
        void Construct(void*& storage, const std::type_index& typeIndex, FormatT& format);
        void* CreateStorage(const std::type_index& typeIndex);

        Type OutputTypeFor(const std::type_index& typeIndex);
        std::type_index TypeIndexFor(const Type& type);

        template<class T>
        void Register(FormatT& format);
    private:
        class Entry
        {
        public:
            const Type outputType;
            const std::vector<Type> inputTypes;
            const std::type_index typeIndex;
        public:
            virtual ~Entry() = 0;

            virtual void Save(const void* object, FormatT& format) = 0;
            virtual void Load(void* object, FormatT& format) = 0;
            virtual void Construct(void*& storage, FormatT& format) = 0;
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

            void Save(const void* object, FormatT& format) override;
            void Load(void* object, FormatT& format) override;
            void Construct(void*& storage, FormatT& format) override;
            [[nodiscard]] void* CreateStorage() const override;
        private:
            using DerivedT = remove_const_t<T>;
            using DerivedScribe = Scribe<DerivedT>;
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
        TrackingID RequiredTypeTrackingIDFor(const T* object, FormatT& format);
    };

    template<class Format>
    void PolymorphicManager<Format>::Scriven(void* object, const std::type_index& typeIndex, FormatT& format)
    {
        if (format.IsOutput())
            Save(object, typeIndex, format);
        else
            Load(object, typeIndex, format);
    }

    template<class Format>
    template<class T>
    void PolymorphicManager<Format>::Save(const T* object, FormatT& format)
    {
        auto typeID = RequiredTypeTrackingIDFor(object, format);
        auto found = FindRequiredEntry(typeid(*RemoveConst(object)));
        (*found)->Save(object, format);
    }

    template<class Format>
    void PolymorphicManager<Format>::Save(void* object, const std::type_index& typeIndex, FormatT& format)
    {
        auto found = FindRequiredEntry(typeIndex);
        (*found)->Save(object, format);
    }

    template<class Format>
    void PolymorphicManager<Format>::Load(void* object, const std::type_index& typeIndex, FormatT& format)
    {
        auto found = FindRequiredEntry(typeIndex);
        (*found)->Load(object, format);
    }

    template<class Format>
    void PolymorphicManager<Format>::Construct(
        void*& storage, const std::type_index& typeIndex, FormatT& format)
    {
        auto found = FindRequiredEntry(typeIndex);
        (*found)->Construct(storage, format);
    }

    template<class Format>
    void* PolymorphicManager<Format>::CreateStorage(
        const std::type_index& typeIndex)
    {
        auto found = FindRequiredEntry(typeIndex);
        return (*found)->CreateStorage();
    }

    template<class Format>
    Type PolymorphicManager<Format>::OutputTypeFor(const std::type_index& typeIndex)
    {
        auto found = FindRequiredEntry(typeIndex);
        return (*found)->outputType;
    }

    template<class Format>
    auto PolymorphicManager<Format>::TypeIndexFor(const Type& type) -> std::type_index
    {
        auto found = FindRequiredEntry(type);
        return (*found)->typeIndex;
    }

    template<class Format>
    template<class T>
    void PolymorphicManager<Format>::Register(FormatT& format)
    {
        auto newInputTypes = InputTypesFor<T>(format);

        std::vector<Type> duplicateTypes;
        for (auto& currentInput : inputTypes)
            for (auto& currentNewInput : newInputTypes)
                if (currentNewInput == currentInput)
                    duplicateTypes.push_back(currentNewInput);

        if (!duplicateTypes.empty())
            throw InputTypesAlreadyRegistered(duplicateTypes);

        auto outputType = ::Inscription::OutputTypeFor<T>(format);
        entryList.push_back(std::make_unique<EntryDerived<T>>(outputType, newInputTypes));
        std::move
        (
            newInputTypes.begin(),
            newInputTypes.end(),
            std::back_inserter(inputTypes)
        );
    }

    template<class Format>
    PolymorphicManager<Format>::Entry::~Entry() = default;

    template<class Format>
    PolymorphicManager<Format>::Entry::Entry(
        Type outputType,
        std::vector<Type> inputTypes,
        std::type_index typeIndex)
        :
        outputType(outputType), inputTypes(inputTypes), typeIndex(typeIndex)
    {}

    template<class Format>
    template<class T>
    PolymorphicManager<Format>::EntryDerived<T>::EntryDerived(
        Type outputType, std::vector<Type> inputTypes)
        :
        Entry(outputType, inputTypes, typeid(T))
    {}

    template<class Format>
    template<class T>
    void PolymorphicManager<Format>::EntryDerived<T>::Save(const void* object, FormatT& format)
    {
        auto castedObject = reinterpret_cast<const T*>(object);
        ScrivenDispatch::Execute(RemoveConst(*castedObject), format);
    }

    template<class Format>
    template<class T>
    void PolymorphicManager<Format>::EntryDerived<T>::Load(void* object, FormatT& format)
    {
        auto castedObject = reinterpret_cast<T*>(object);
        ScrivenDispatch::Execute(RemoveConst(*castedObject), format);
    }

    template<class Format>
    template<class T>
    void PolymorphicManager<Format>::EntryDerived<T>::Construct(void*& storage, FormatT& format)
    {
        DerivedT* castedStorage = reinterpret_cast<DerivedT*>(storage);
        ConstructDispatch::Execute(castedStorage, format);
    }

    template<class Format>
    template<class T>
    void* PolymorphicManager<Format>::EntryDerived<T>::CreateStorage() const
    {
        return ::Inscription::CreateStorage(sizeof(T));
    }

    template<class Format>
    auto PolymorphicManager<Format>::FindRequiredEntry(const Type& type)
        -> EntryIterator
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

    template<class Format>
    auto PolymorphicManager<Format>::FindRequiredEntry(const std::type_index& typeIndex)
        -> EntryIterator
    {
        for (auto loop = entryList.begin(); loop != entryList.end(); ++loop)
            if ((*loop)->typeIndex == typeIndex)
                return loop;

        throw InputTypeNotFound(typeIndex);
    }

    template<class Format>
    template<class T>
    TrackingID PolymorphicManager<Format>::RequiredTypeTrackingIDFor(const T* object, FormatT& format)
    {
        auto& type = typeid(*RemoveConst(object));
        auto found = format.types.FindTypeID(type);
        if (found.has_value())
            return *found;

        throw InputTypeNotFound(type);
    }
}