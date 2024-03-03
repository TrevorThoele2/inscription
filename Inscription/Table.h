#pragma once

#include <type_traits>
#include <typeindex>
#include <string>
#include <memory>
#include <unordered_map>
#include <tuple>

#include "TableEntryBasic.h"
#include "TableEntryOwningPointer.h"
#include "TableEntryUnowningPointer.h"
#include "TableEntryInscripter.h"
#include "TableLinkable.h"
#include "TableLinkableClass.h"
#include "TableLinkableNonClass.h"
#include "TableImposterClass.h"
#include "TableImposterNonClass.h"
#include "TableEntryBase.h"

#include "TableNotConstructedException.h"
#include "TableVariableNotHereException.h"

#include "ScopedConstructor.h"
#include "Access.h"
#include "Const.h"

#include "Exception.h"
#include "Assert.h"

namespace Inscription
{
    template<class ClassT>
    class Table;

    class Scribe;
    template<class ClassT>
    class TableBase
    {
    public:
        typedef ClassT ClassT;
        typedef std::string Name;
    public:
        TableBase();
        TableBase(const TableBase& arg);
        TableBase& operator=(const TableBase& arg);
        TableBase(TableBase&& arg);
        TableBase& operator=(TableBase&& arg);

        // Basic entries
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
        const Table<BaseT>& Convert() const;

        template<class BaseT>
        void AddBase();

        void Serialize(Scribe& scribe, ClassT& obj);
        void Construct(Scribe& scribe);
        // Resets the entries and scribe
        void Reset();

        Scribe* GetScribe() const;
    private:
        template<class T>
        using LinkableT = typename RemoveConstTrait<typename std::remove_reference<T>::type>::type;
    private:
        typedef TableEntry<ClassT> Entry;
        typedef std::unique_ptr<Entry> EntryPtr;
        typedef std::unordered_map<Name, EntryPtr> EntryMap;
        EntryMap entries;
        typedef std::vector<typename EntryMap::iterator> EntryVector;
        EntryVector entryVector;

        typedef TableEntryBase<ClassT> Base;
        typedef std::unique_ptr<Base> BasePtr;
        typedef std::unordered_map<std::type_index, BasePtr> BaseMap;
        BaseMap bases;
        typedef std::vector<typename BaseMap::iterator> BaseVector;
        BaseVector baseVector;

        Scribe* scribe;
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
        TableEntryBaseDerived<ClassT, BaseT>* FindBase() const;
    };

    template<class ClassT>
    TableBase<ClassT>::TableBase() : scribe(nullptr)
    {}

    template<class ClassT>
    TableBase<ClassT>::TableBase(const TableBase& arg) : scribe(nullptr)
    {
        for (auto &loop : arg.baseVector)
            baseVector.push_back(bases.emplace(loop->first, loop->second->Clone()).first);

        for (auto &loop : arg.entryVector)
            entryVector.push_back(entries.emplace(loop->first, loop->second->Clone()).first);
    }

    template<class ClassT>
    TableBase<ClassT>& TableBase<ClassT>::operator=(const TableBase& arg)
    {
        for (auto &loop : arg.baseVector)
            baseVector.push_back(bases.emplace(loop->first, loop->second->Clone()).first);

        for (auto &loop : arg.entryVector)
            entryVector.push_back(entries.emplace(loop->first, loop->second->Clone()).first);

        return *this;
    }

    template<class ClassT>
    TableBase<ClassT>::TableBase(TableBase&& arg) : scribe(arg.scribe)
    {
        for (auto &loop : arg.baseVector)
            baseVector.push_back(bases.emplace(std::move(loop->first), std::move(loop->second)).first);

        for (auto &loop : arg.entryVector)
            entryVector.push_back(entries.emplace(std::move(loop->first), std::move(loop->second)).first);
    }

    template<class ClassT>
    TableBase<ClassT>& TableBase<ClassT>::operator=(TableBase&& arg)
    {
        for (auto &loop : arg.baseVector)
            baseVector.push_back(bases.emplace(std::move(loop->first), std::move(loop->second)).first);

        for (auto &loop : arg.entryVector)
            entryVector.push_back(entries.emplace(std::move(loop->first), std::move(loop->second)).first);

        return *this;
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ClassT>::Add(const Name& name)
    {
        AddImplementation<T, var>(name);
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ClassT>::AddOwningPointer(const Name& name)
    {
        static_assert(std::is_pointer<T>::value, "The type given MUST be a pointer.");
        entryVector.push_back(entries.emplace(name, EntryPtr(new TableEntryOwningPointer<ClassT, T, var>())).first);
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ClassT>::AddUnowningPointer(const Name& name)
    {
        static_assert(std::is_pointer<T>::value, "The type given MUST be a pointer.");
        entryVector.push_back(entries.emplace(name, EntryPtr(new TableEntryUnowningPointer<ClassT, T, var>())).first);
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::AddLinkable(const Name& name)
    {
        AddLinkableImplementation<LinkableT<T>>(name);
    }

    template<class ClassT>
    template<class InterfaceT, class FileT>
    void TableBase<ClassT>::AddImposter(const Name& name)
    {
        AddImposterImplementation<LinkableT<InterfaceT>, LinkableT<FileT>>(name);
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::Link(const Name& name, T& arg)
    {
        FindEntry<TableLinkable<ClassT, LinkableT<T>>>(name)->Link(RemoveConst(arg));
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::Link(const Name& name, T&& arg)
    {
        FindEntry<TableLinkable<ClassT, LinkableT<T>>>(name)->LinkMove(std::move(arg));
    }

    template<class ClassT>
    template<class T>
    T& TableBase<ClassT>::Get(const Name& name) const
    {
        return FindEntry<TableEntryInherit<ClassT, T>>(name)->Get();
    }

    template<class ClassT>
    template<class BaseT>
    const Table<BaseT>& TableBase<ClassT>::Convert() const
    {
        return FindBase<BaseT>()->base;
    }

    template<class ClassT>
    template<class BaseT>
    void TableBase<ClassT>::AddBase()
    {
        baseVector.push_back(bases.emplace(typeid(BaseT), BasePtr(new TableEntryBaseDerived<ClassT, BaseT>())).first);
    }

    template<class ClassT>
    void TableBase<ClassT>::Serialize(Scribe& scribe, ClassT& obj)
    {
        Reset();

        this->scribe = &scribe;

        for (auto &loop : baseVector)
            loop->second->Serialize(scribe, obj);

        for (auto &loop : entryVector)
            loop->second->Serialize(scribe, obj);
    }

    template<class ClassT>
    void TableBase<ClassT>::Construct(Scribe& scribe)
    {
        Reset();

        this->scribe = &scribe;

        for (auto &loop : baseVector)
            loop->second->Construct(scribe);

        for (auto &loop : entryVector)
            loop->second->Construct(scribe);
    }

    template<class ClassT>
    void TableBase<ClassT>::Reset()
    {
        for (auto &loop : entries)
            loop.second->Clear();

        scribe = nullptr;
    }

    template<class ClassT>
    Scribe* TableBase<ClassT>::GetScribe() const
    {
        return scribe;
    }

    template<class ClassT>
    template<class T, T ClassT::*var, typename std::enable_if<!Access::InscripterT<T>::exists, int>::type>
    void TableBase<ClassT>::AddImplementation(const Name& name)
    {
        AddEntryGeneral(name, new TableEntryBasic<ClassT, T, var>());
    }

    template<class ClassT>
    template<class T, T ClassT::*var, typename std::enable_if<Access::InscripterT<T>::exists, int>::type>
    void TableBase<ClassT>::AddImplementation(const Name& name)
    {
        AddEntryGeneral(name, new TableEntryInscripter<ClassT, T, var>());
    }

    template<class ClassT>
    template<class T, typename std::enable_if<std::is_class<T>::value, int>::type>
    void TableBase<ClassT>::AddLinkableImplementation(const Name& name)
    {
        AddEntryGeneral(name, new TableLinkableClass<ClassT, T>());
    }

    template<class ClassT>
    template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type>
    void TableBase<ClassT>::AddLinkableImplementation(const Name& name)
    {
        AddEntryGeneral(name, new TableLinkableNonClass<ClassT, T>());
    }

    template<class ClassT>
    template<class InterfaceT, class RealT, typename std::enable_if<std::is_class<RealT>::value, int>::type>
    void TableBase<ClassT>::AddImposterImplementation(const Name& name)
    {
        AddEntryGeneral(name, new TableImposterClass<ClassT, InterfaceT, RealT>());
    }

    template<class ClassT>
    template<class InterfaceT, class RealT, typename std::enable_if<!std::is_class<RealT>::value, int>::type>
    void TableBase<ClassT>::AddImposterImplementation(const Name& name)
    {
        AddEntryGeneral(name, new TableImposterNonClass<ClassT, InterfaceT, RealT>());
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::AddEntryGeneral(const Name& name, T* add)
    {
        entryVector.push_back(entries.emplace(name, EntryPtr(add)).first);
    }

    template<class ClassT>
    template<class T>
    T* TableBase<ClassT>::FindEntry(const Name& name) const
    {
        auto found = entries.find(name);
        if (found == entries.end())
            throw TableVariableNotHereException();

#ifndef INSCRIPTION_DEBUG
        return static_cast<T*>(found->second.get());
#else
        T *casted = dynamic_cast<T*>(found->second.get());
        INSCRIPTION_ASSERT_MESSAGE(casted != nullptr, "An entry was attempted to be accessed with the incorrect type.");
        return casted;
#endif
    }

    template<class ClassT>
    template<class BaseT>
    TableEntryBaseDerived<ClassT, BaseT>* TableBase<ClassT>::FindBase() const
    {
        auto found = bases.find(typeid(BaseT));
        if (found == bases.end())
            throw TableVariableNotHereException();

#ifndef INSCRIPTION_DEBUG
        return static_cast<TableEntryBaseDerived<ClassT, BaseT>*>(found->second.get());
#else
        BaseDerived<BaseT> *casted = dynamic_cast<BaseDerived<BaseT>*>(found->second.get());
        INSCRIPTION_ASSERT_MESSAGE(casted != nullptr, "A base was attempted to be accessed with the incorrect type.");
        return casted;
#endif
    }

    template<class ClassT>
    class Table : public TableBase<ClassT>
    {
    public:
        Table() = default;
        Table(const Table& arg);
        Table& operator=(const Table& arg);
        Table(Table&& arg);
        Table& operator=(Table&& arg);
    };

    template<class ClassT>
    Table<ClassT>::Table(const Table& arg) : TableBase(arg)
    {}

    template<class ClassT>
    Table<ClassT>& Table<ClassT>::operator=(const Table& arg)
    {
        TableBase<ClassT>::operator=(arg);
        return *this;
    }

    template<class ClassT>
    Table<ClassT>::Table(Table&& arg) : TableBase<ClassT>(std::move(arg))
    {}

    template<class ClassT>
    Table<ClassT>& Table<ClassT>::operator=(Table&& arg)
    {
        TableBase<ClassT>::operator=(std::move(arg));
        return *this;
    }
}

#include "UndefAssert.h"