#pragma once

#include <type_traits>
#include <typeindex>
#include <string>
#include <memory>
#include <unordered_map>
#include <tuple>

#include "BasicTableEntry.h"
#include "OwningPointerTableEntry.h"
#include "UnowningPointerTableEntry.h"
#include "InscripterTableEntry.h"
#include "LinkableTableEntry.h"
#include "ClassLinkableTableEntry.h"
#include "NonClassLinkableTableEntry.h"
#include "ClassTableImposter.h"
#include "NonClassTableImposter.h"
#include "BaseClassTableEntry.h"

#include "TableNotConstructed.h"
#include "TableVariableNotHere.h"
#include "BaseClassNotInTable.h"

#include "ScopedConstructor.h"
#include "Access.h"
#include "Const.h"

#include "Exception.h"
#include "Assert.h"

namespace Inscription
{
    template<class ScribeT, class ClassT>
    class Table;
    class Scribe;

    template<class ScribeT, class ClassT>
    class TableBase
    {
    public:
        typedef ClassT ClassT;
        typedef ScribeT ScribeT;
        typedef std::string Name;
    public:
        TableBase();
        TableBase(const TableBase& arg);
        TableBase(TableBase&& arg);
        TableBase& operator=(const TableBase& arg);
        TableBase& operator=(TableBase&& arg);

        // Basic entryMap
        template<class T, T ClassT::*var>
        void Add(const Name& name);
        template<class T, T ClassT::*var>
        void AddOwningPointer(const Name& name);
        template<class T, T ClassT::*var>
        void AddUnowningPointer(const Name& name);

        // Linkables
        // Must be linked from the serialize function
        template<class T>
        void AddLinkable(const Name& name);
        // Imposters are mainly used for versioning
        // InterfaceT should be the type you're going to extract out of here and link to
        // FileT is what is going to be extracted out of the file
        // This uses inscription::Convert to create a FileT out of an InterfaceT
        // This uses inscription::ConvertHeap to construct an InterfaceT out of a FileT
        // This uses inscription::Assign to assign an InterfaceT to a FileT
        template<class InterfaceT, class FileT>
        void AddImposter(const Name& name);
        // Link up references or externals from inside of the serialization function
        template<class T>
        void Link(const Name& name, T& arg);
        // Link up references or externals from inside of the serialization function
        template<class T>
        void Link(const Name& name, T&& arg);

        // Get a variable for construction
        template<class T>
        T& Get(const Name& name) const;
        // Retrieves a base class' serialization table for use in passing to constructors
        template<class BaseT>
        const Table<ScribeT, BaseT>& Convert() const;

        template<class BaseT>
        void AddBase();

        void Serialize(ScribeT& scribe, ClassT& obj);
        void Construct(ScribeT& scribe);
        // Resets the entryMap and scribe
        void Reset();

        ScribeT* GetScribe() const;
    private:
        template<class T>
        using StorableInLink = typename RemoveConstTrait<typename std::remove_reference<T>::type>::type;
    private:
        using Entry = TableEntry<ScribeT, ClassT>;
        typedef std::unique_ptr<Entry> EntryPtr;
        typedef std::unordered_map<Name, EntryPtr> EntryMap;
        EntryMap entryMap;
        typedef std::vector<typename EntryMap::iterator> EntryList;
        EntryList entryList;

        using BaseClassEntryBase = BaseClassTableEntryBase<ScribeT, ClassT>;
        typedef std::unique_ptr<BaseClassEntryBase> BaseClassEntryBasePtr;
        typedef std::unordered_map<std::type_index, BaseClassEntryBasePtr> BaseClassEntryBaseMap;
        BaseClassEntryBaseMap baseClassMap;
        typedef std::vector<typename BaseClassEntryBaseMap::iterator> BaseClassEntryBaseList;
        BaseClassEntryBaseList baseClassList;
    private:
        template<class T, T ClassT::*var>
        using BasicEntry = BasicTableEntry<ScribeT, ClassT, T, var>;
        template<class T>
        using GettableEntry = GettableTableEntry<ScribeT, ClassT, T>;
        template<class T, T ClassT::*var>
        using InscripterEntry = InscripterTableEntry<ScribeT, ClassT, T, var>;
        template<class T, T ClassT::*var>
        using OwningPointerEntry = OwningPointerTableEntry<ScribeT, ClassT, T, var>;
        template<class T, T ClassT::*var>
        using UnowningPointerEntry = UnowningPointerTableEntry<ScribeT, ClassT, T, var>;
        template<class T>
        using BaseClassEntry = BaseClassTableEntry<ScribeT, ClassT, T>;
        template<class T>
        using LinkableEntry = LinkableTableEntry<ScribeT, ClassT, T>;
        template<class T>
        using ClassLinkableEntry = ClassLinkableTableEntry<ScribeT, ClassT, T>;
        template<class T>
        using NonClassLinkableEntry = NonClassLinkableTableEntry<ScribeT, ClassT, T>;
        template<class InterfaceT, class FileT>
        using ClassImposter = ClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>;
        template<class InterfaceT, class FileT>
        using NonClassImposter = NonClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>;
    private:
        ScribeT* scribe;
    private:
        template<class T, T ClassT::*var, typename std::enable_if<!Access::InscripterT<T>::exists, int>::type = 0>
        void AddImplementation(const Name& name);
        template<class T, T ClassT::*var, typename std::enable_if<Access::InscripterT<T>::exists, int>::type = 0>
        void AddImplementation(const Name& name);

        template<class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
        void AddLinkableImplementation(const Name& name);
        template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type = 0>
        void AddLinkableImplementation(const Name& name);

        template<class InterfaceT, class RealT, typename std::enable_if<std::is_class<RealT>::value, int>::type = 0>
        void AddImposterImplementation(const Name& name);
        template<class InterfaceT, class RealT, typename std::enable_if<!std::is_class<RealT>::value, int>::type = 0>
        void AddImposterImplementation(const Name& name);

        template<class T>
        void AddEntryGeneral(const Name& name, T* add);
        template<class T>
        T* FindEntry(const Name& name) const;
        template<class BaseT>
        BaseClassEntry<BaseT>* FindBase() const;
    };

    template<class ScribeT, class ClassT>
    TableBase<ScribeT, ClassT>::TableBase() : scribe(nullptr)
    {}

    template<class ScribeT, class ClassT>
    TableBase<ScribeT, ClassT>::TableBase(const TableBase& arg) : scribe(nullptr)
    {
        for (auto& loop : arg.baseClassList)
            baseClassList.push_back(baseClassMap.emplace(loop->first, loop->second->Clone()).first);

        for (auto& loop : arg.entryList)
            entryList.push_back(entryMap.emplace(loop->first, loop->second->Clone()).first);
    }

    template<class ScribeT, class ClassT>
    TableBase<ScribeT, ClassT>::TableBase(TableBase&& arg) : scribe(arg.scribe)
    {
        for (auto& loop : arg.baseClassList)
            baseClassList.push_back(baseClassMap.emplace(std::move(loop->first), std::move(loop->second)).first);

        for (auto& loop : arg.entryList)
            entryList.push_back(entryMap.emplace(std::move(loop->first), std::move(loop->second)).first);
    }

    template<class ScribeT, class ClassT>
    TableBase<ScribeT, ClassT>& TableBase<ScribeT, ClassT>::operator=(const TableBase& arg)
    {
        for (auto& loop : arg.baseClassList)
            baseClassList.push_back(baseClassMap.emplace(loop->first, loop->second->Clone()).first);

        for (auto& loop : arg.entryList)
            entryList.push_back(entryMap.emplace(loop->first, loop->second->Clone()).first);

        return *this;
    }

    template<class ScribeT, class ClassT>
    TableBase<ScribeT, ClassT>& TableBase<ScribeT, ClassT>::operator=(TableBase&& arg)
    {
        for (auto& loop : arg.baseClassList)
            baseClassList.push_back(baseClassMap.emplace(std::move(loop->first), std::move(loop->second)).first);

        for (auto& loop : arg.entryList)
            entryList.push_back(entryMap.emplace(std::move(loop->first), std::move(loop->second)).first);

        return *this;
    }

    template<class ScribeT, class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ScribeT, ClassT>::Add(const Name& name)
    {
        AddImplementation<T, var>(name);
    }

    template<class ScribeT, class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ScribeT, ClassT>::AddOwningPointer(const Name& name)
    {
        static_assert(std::is_pointer<T>::value, "The type given MUST be a pointer.");
        entryList.push_back(entryMap.emplace(name, EntryPtr(new OwningPointerEntry<T, var>())).first);
    }

    template<class ScribeT, class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ScribeT, ClassT>::AddUnowningPointer(const Name& name)
    {
        static_assert(std::is_pointer<T>::value, "The type given MUST be a pointer.");
        entryList.push_back(entryMap.emplace(name, EntryPtr(new UnowningPointerEntry<T, var>())).first);
    }

    template<class ScribeT, class ClassT>
    template<class T>
    void TableBase<ScribeT, ClassT>::AddLinkable(const Name& name)
    {
        AddLinkableImplementation<StorableInLink<T>>(name);
    }

    template<class ScribeT, class ClassT>
    template<class InterfaceT, class FileT>
    void TableBase<ScribeT, ClassT>::AddImposter(const Name& name)
    {
        AddImposterImplementation<StorableInLink<InterfaceT>, StorableInLink<FileT>>(name);
    }

    template<class ScribeT, class ClassT>
    template<class T>
    void TableBase<ScribeT, ClassT>::Link(const Name& name, T& arg)
    {
        FindEntry<LinkableEntry<StorableInLink<T>>>(name)->Link(RemoveConst(arg));
    }

    template<class ScribeT, class ClassT>
    template<class T>
    void TableBase<ScribeT, ClassT>::Link(const Name& name, T&& arg)
    {
        FindEntry<LinkableEntry<StorableInLink<T>>>(name)->LinkMove(std::move(arg));
    }

    template<class ScribeT, class ClassT>
    template<class T>
    T& TableBase<ScribeT, ClassT>::Get(const Name& name) const
    {
        return FindEntry<GettableEntry<T>>(name)->Get();
    }

    template<class ScribeT, class ClassT>
    template<class BaseT>
    const Table<ScribeT, BaseT>& TableBase<ScribeT, ClassT>::Convert() const
    {
        return FindBase<BaseT>()->base;
    }

    template<class ScribeT, class ClassT>
    template<class BaseT>
    void TableBase<ScribeT, ClassT>::AddBase()
    {
        auto emplaced = baseClassMap.emplace(typeid(BaseT), BaseClassEntryBasePtr(new BaseClassEntry<BaseT>()));
        baseClassList.push_back(emplaced.first);
    }

    template<class ScribeT, class ClassT>
    void TableBase<ScribeT, ClassT>::Serialize(ScribeT& scribe, ClassT& obj)
    {
        Reset();

        this->scribe = &scribe;

        for (auto& loop : baseClassList)
            loop->second->Serialize(scribe, obj);

        for (auto& loop : entryList)
            loop->second->Serialize(scribe, obj);
    }

    template<class ScribeT, class ClassT>
    void TableBase<ScribeT, ClassT>::Construct(ScribeT& scribe)
    {
        Reset();

        this->scribe = &scribe;

        for (auto& loop : baseClassList)
            loop->second->Construct(scribe);

        for (auto& loop : entryList)
            loop->second->Construct(scribe);
    }

    template<class ScribeT, class ClassT>
    void TableBase<ScribeT, ClassT>::Reset()
    {
        for (auto& loop : entryMap)
            loop.second->Clear();

        scribe = nullptr;
    }

    template<class ScribeT, class ClassT>
    ScribeT* TableBase<ScribeT, ClassT>::GetScribe() const
    {
        return scribe;
    }

    template<class ScribeT, class ClassT>
    template<class T, T ClassT::*var, typename std::enable_if<!Access::InscripterT<T>::exists, int>::type>
    void TableBase<ScribeT, ClassT>::AddImplementation(const Name& name)
    {
        AddEntryGeneral(name, new BasicEntry<T, var>());
    }

    template<class ScribeT, class ClassT>
    template<class T, T ClassT::*var, typename std::enable_if<Access::InscripterT<T>::exists, int>::type>
    void TableBase<ScribeT, ClassT>::AddImplementation(const Name& name)
    {
        AddEntryGeneral(name, new InscripterEntry<T, var>());
    }

    template<class ScribeT, class ClassT>
    template<class T, typename std::enable_if<std::is_class<T>::value, int>::type>
    void TableBase<ScribeT, ClassT>::AddLinkableImplementation(const Name& name)
    {
        AddEntryGeneral(name, new ClassLinkableEntry<T>());
    }

    template<class ScribeT, class ClassT>
    template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type>
    void TableBase<ScribeT, ClassT>::AddLinkableImplementation(const Name& name)
    {
        AddEntryGeneral(name, new NonClassLinkableEntry<T>());
    }

    template<class ScribeT, class ClassT>
    template<class InterfaceT, class RealT, typename std::enable_if<std::is_class<RealT>::value, int>::type>
    void TableBase<ScribeT, ClassT>::AddImposterImplementation(const Name& name)
    {
        AddEntryGeneral(name, new ClassImposter<InterfaceT, RealT>());
    }

    template<class ScribeT, class ClassT>
    template<class InterfaceT, class RealT, typename std::enable_if<!std::is_class<RealT>::value, int>::type>
    void TableBase<ScribeT, ClassT>::AddImposterImplementation(const Name& name)
    {
        AddEntryGeneral(name, new NonClassImposter<InterfaceT, RealT>());
    }

    template<class ScribeT, class ClassT>
    template<class T>
    void TableBase<ScribeT, ClassT>::AddEntryGeneral(const Name& name, T* add)
    {
        entryList.push_back(entryMap.emplace(name, EntryPtr(add)).first);
    }

    template<class ScribeT, class ClassT>
    template<class T>
    T* TableBase<ScribeT, ClassT>::FindEntry(const Name& name) const
    {
        auto found = entryMap.find(name);
        if (found == entryMap.end())
            throw TableVariableNotHere(name);

#ifndef INSCRIPTION_DEBUG
        return static_cast<T*>(found->second.get());
#else
        T *casted = dynamic_cast<T*>(found->second.get());
        INSCRIPTION_ASSERT_MESSAGE(casted != nullptr, "An entry was attempted to be accessed with the incorrect type.");
        return casted;
#endif
    }

    template<class ScribeT, class ClassT>
    template<class BaseT>
    typename TableBase<ScribeT, ClassT>::BaseClassEntry<BaseT>* TableBase<ScribeT, ClassT>::FindBase() const
    {
        auto found = baseClassMap.find(typeid(BaseT));
        if (found == baseClassMap.end())
            throw BaseClassNotInTable(typeid(BaseT));

#ifndef INSCRIPTION_DEBUG
        return static_cast<BaseClassEntry<BaseT>*>(found->second.get());
#else
        BaseDerived<BaseT> *casted = dynamic_cast<BaseDerived<BaseT>*>(found->second.get());
        INSCRIPTION_ASSERT_MESSAGE(casted != nullptr, "A base was attempted to be accessed with the incorrect type.");
        return casted;
#endif
    }
}

#include "UndefAssert.h"