#pragma once

#include <type_traits>
#include <typeindex>
#include <string>
#include <memory>
#include <unordered_map>
#include <tuple>

#include "StackConstructor.h"
#include "Access.h"
#include "Const.h"

#include "Exception.h"
#include "Assert.h"

namespace Inscription
{
    class TableNotLinkedException : public Exception
    {
    public:
        TableNotLinkedException();
    };

    class TableNotConstructedException : public Exception
    {
    public:
        TableNotConstructedException();
    };

    class TableVariableNotHereException : public Exception
    {
    public:
        TableVariableNotHereException();
    };

    // Convert
    template<class Original, class Converted>
    Converted Convert(const Original &orig)
    {
        return Converted(orig);
    }

    // ConvertHeap
    template<class Original, class Converted>
    Converted* ConvertHeap(const Original &orig)
    {
        return new Converted(orig);
    }

    // Assign
    template<class Left, class Right>
    void Assign(Left &lhs, const Right &rhs)
    {
        lhs = rhs;
    }

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
        TableBase(const TableBase &arg);
        TableBase& operator=(const TableBase &arg);
        TableBase(TableBase &&arg);
        TableBase& operator=(TableBase &&arg);

        // Basic entries
        template<class T, T ClassT::*var>
        void Add(const Name &name);
        template<class T, T ClassT::*var>
        void AddAssumeExists(const Name &name);
        template<class T, T ClassT::*var>
        void AddSimplePointer(const Name &name);

        // Linkables
        // Must be linked from the serialize function
        template<class T>
        void AddLinkable(const Name &name);
        // Imposters are mainly used for versioning
        // InterfaceT should be the type you're going to extract out of here and link to
        // FileT is what is going to be extracted out of the file
        // This uses inscription::Convert to create a FileT out of an InterfaceT
        // This uses inscription::ConvertHeap to construct an InterfaceT out of a FileT
        // This uses inscription::Assign to assign an InterfaceT to a FileT
        template<class InterfaceT, class FileT>
        void AddImposter(const Name &name);
        // Link up references or externals from inside of the serialization function
        template<class T>
        void Link(const Name &name, T &arg);
        // Link up references or externals from inside of the serialization function
        template<class T>
        void Link(const Name &name, T &&arg);

        // Get a variable for construction
        template<class T>
        T& Get(const Name &name) const;
        // Retrieves a base class' serialization table for use in passing to constructors
        template<class BaseT>
        const Table<BaseT>& Convert() const;

        template<class BaseT>
        void AddBase();

        void Serialize(Scribe &scribe, ClassT &obj);
        void Construct(Scribe &scribe);
        // Resets the entries and scribe
        void Reset();

        Scribe* GetScribe() const;
    private:
        class Entry
        {
        public:
            virtual ~Entry() = 0;
            virtual Entry* Clone() = 0;

            virtual void Serialize(Scribe &scribe, ClassT &obj) = 0;
            virtual void Construct(Scribe &scribe) = 0;
            virtual void Clear() = 0;
        };

        //////////////////
        // REGULAR ENTRIES

        template<class T>
        class EntryInherit : public Entry
        {
        public:
            virtual T& Get() = 0;
        };

        template<class T, T ClassT::*var>
        class Basic : public EntryInherit<T>
        {
        private:
            typedef typename RemoveConstTrait<T>::type StoreT;
        public:
            Basic* Clone() override;

            T& Get() override;

            void Serialize(Scribe &scribe, ClassT &obj) override;
            void Construct(Scribe &scribe) override;
            void Clear() override;
        private:
            std::unique_ptr<StackConstructor<StoreT>> constructor;
        };

        template<class T, T ClassT::*var>
        class BasicAssumeExists : public EntryInherit<T>
        {
        private:
            typedef typename RemoveConstTrait<T>::type StoreT;
        public:
            BasicAssumeExists();
            BasicAssumeExists* Clone() override;

            T& Get() override;

            void Serialize(Scribe &scribe, ClassT &obj) override;
            void Construct(Scribe &scribe) override;
            void Clear() override;
        private:
            // Will already be a pointer
            T obj;
        };

        template<class T, T ClassT::*var>
        class BasicSimplePointer : public EntryInherit<T>
        {
        private:
            typedef typename RemoveConstTrait<T>::type StoreT;
        public:
            BasicSimplePointer();
            BasicSimplePointer* Clone() override;

            T& Get() override;

            void Serialize(Scribe &scribe, ClassT &obj) override;
            void Construct(Scribe &scribe) override;
            void Clear() override;
        private:
            // Will already be a pointer
            T obj;
        };

        template<class T, T ClassT::*var>
        class BasicInscripter : public EntryInherit<T>
        {
        private:
            typedef typename RemoveConstTrait<T>::type StoreT;
        public:
            BasicInscripter* Clone() override;

            T& Get() override;
            void Serialize(Scribe &scribe, ClassT &obj) override;
            void Construct(Scribe &scribe) override;
            void Clear() override;
        private:
            std::unique_ptr<StoreT> stored;
        };

        ////////////
        // LINKABLES

        template<class T>
        class LinkableBase : public EntryInherit<T>
        {
        public:
            virtual ~LinkableBase() = 0;

            void Serialize(Scribe &scribe, ClassT &obj) override;
            void Construct(Scribe &scribe) override;

            void Clear() override;
            virtual void Link(T &obj) = 0;
            virtual void LinkMove(T &&obj) = 0;
        private:
            virtual void SerializeImpl(Scribe &scribe, ClassT &obj) = 0;
            virtual void ConstructImpl(Scribe &scribe) = 0;
            virtual bool IsValid() const = 0;
        };

        template<class T>
        class LinkableForClass : public LinkableBase<T>
        {
        public:
            LinkableForClass();
            LinkableForClass* Clone() override;

            T& Get() override;
            void Link(T &obj) override;
            void LinkMove(T &&obj) override;
        private:
            T *var;
            void SerializeImpl(Scribe &scribe, ClassT &obj) override;
            void ConstructImpl(Scribe &scribe) override;
            bool IsValid() const override;
        };

        template<class T>
        class LinkableForNonClass : public LinkableBase<T>
        {
        public:
            LinkableForNonClass();
            LinkableForNonClass* Clone() override;

            T& Get() override;
            void Link(T &obj) override;
            void LinkMove(T &&obj) override;
        private:
            bool valid;
            T var;

            void SerializeImpl(Scribe &scribe, ClassT &obj) override;
            void ConstructImpl(Scribe &scribe) override;
            bool IsValid() const;
        };

        template<class T>
        using LinkableT = typename RemoveConstTrait<typename std::remove_reference<T>::type>::type;

        // IMPOSTERS
        template<class InterfaceT, class FileT>
        class ImposterForClass : public LinkableBase<InterfaceT>
        {
        public:
            ImposterForClass();
            ImposterForClass(const ImposterForClass &arg);
            ImposterForClass& operator=(const ImposterForClass &arg) = delete;
            ~ImposterForClass();
            ImposterForClass* Clone() override;

            InterfaceT& Get() override;
            void Link(InterfaceT &obj) override;
            void LinkMove(InterfaceT &&obj) override;
        private:
            std::unique_ptr<InterfaceT> interfaceVar;
            bool deleteInterfaceVar;
            void SerializeImpl(Scribe &scribe, ClassT &obj) override;
            void ConstructImpl(Scribe &scribe) override;
            bool IsValid() const override;
        };

        template<class InterfaceT, class FileT>
        class ImposterForNonClass : public LinkableBase<InterfaceT>
        {
        public:
            ImposterForNonClass();
            ImposterForNonClass* Clone() override;

            InterfaceT& Get() override;
            void Link(InterfaceT &obj) override;
            void LinkMove(InterfaceT &&obj) override;
        private:
            bool valid;
            InterfaceT interfaceVar;

            void SerializeImpl(Scribe &scribe, ClassT &obj) override;
            void ConstructImpl(Scribe &scribe) override;
            bool IsValid() const;
        };

        ////////
        // BASES

        class Base
        {
        public:
            virtual ~Base() = 0;
            virtual Base* Clone() = 0;

            virtual void Serialize(Scribe &scribe, ClassT &obj) = 0;
            virtual void Construct(Scribe &scribe) = 0;
        };

        template<class T>
        class BaseDerived : public Base
        {
        public:
            Table<T> base;
            BaseDerived() = default;
            BaseDerived* Clone() override;

            void Serialize(Scribe &scribe, ClassT &obj) override;
            void Construct(Scribe &scribe) override;
        };
    private:
        typedef std::unique_ptr<Entry> EntryPtr;
        typedef std::unordered_map<Name, EntryPtr> EntryMap;
        EntryMap entries;
        typedef std::vector<typename EntryMap::iterator> EntryVector;
        EntryVector entryVector;

        typedef std::unique_ptr<Base> BasePtr;
        typedef std::unordered_map<std::type_index, BasePtr> BaseMap;
        BaseMap bases;
        typedef std::vector<typename BaseMap::iterator> BaseVector;
        BaseVector baseVector;

        Scribe *scribe;

        template<class T, T ClassT::*var, typename std::enable_if<!Access::InscripterT<T>::exists, int>::type = 0>
        void AddImplementation(const Name &name);
        template<class T, T ClassT::*var, typename std::enable_if<Access::InscripterT<T>::exists, int>::type = 0>
        void AddImplementation(const Name &name);

        template<class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
        void AddLinkableImplementation(const Name &name);
        template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type = 0>
        void AddLinkableImplementation(const Name &name);

        template<class InterfaceT, class RealT, typename std::enable_if<std::is_class<RealT>::value, int>::type = 0>
        void AddImposterImplementation(const Name &name);
        template<class InterfaceT, class RealT, typename std::enable_if<!std::is_class<RealT>::value, int>::type = 0>
        void AddImposterImplementation(const Name &name);

        template<class T>
        void AddEntryGeneral(const Name &name, T *add);
        template<class T>
        T* FindEntry(const Name &name) const;
        template<class BaseT>
        BaseDerived<BaseT>* FindBase() const;
    };

    template<class ClassT>
    TableBase<ClassT>::TableBase() : scribe(nullptr)
    {}

    template<class ClassT>
    TableBase<ClassT>::TableBase(const TableBase &arg) : scribe(nullptr)
    {
        for (auto &loop : arg.baseVector)
            baseVector.push_back(bases.emplace(loop->first, loop->second->Clone()).first);

        for (auto &loop : arg.entryVector)
            entryVector.push_back(entries.emplace(loop->first, loop->second->Clone()).first);
    }

    template<class ClassT>
    TableBase<ClassT>& TableBase<ClassT>::operator=(const TableBase &arg)
    {
        for (auto &loop : arg.baseVector)
            baseVector.push_back(bases.emplace(loop->first, loop->second->Clone()).first);

        for (auto &loop : arg.entryVector)
            entryVector.push_back(entries.emplace(loop->first, loop->second->Clone()).first);

        return *this;
    }

    template<class ClassT>
    TableBase<ClassT>::TableBase(TableBase &&arg) : scribe(arg.scribe)
    {
        for (auto &loop : arg.baseVector)
            baseVector.push_back(bases.emplace(std::move(loop->first), std::move(loop->second)).first);

        for (auto &loop : arg.entryVector)
            entryVector.push_back(entries.emplace(std::move(loop->first), std::move(loop->second)).first);
    }

    template<class ClassT>
    TableBase<ClassT>& TableBase<ClassT>::operator=(TableBase &&arg)
    {
        for (auto &loop : arg.baseVector)
            baseVector.push_back(bases.emplace(std::move(loop->first), std::move(loop->second)).first);

        for (auto &loop : arg.entryVector)
            entryVector.push_back(entries.emplace(std::move(loop->first), std::move(loop->second)).first);

        return *this;
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ClassT>::Add(const Name &name)
    {
        AddImplementation<T, var>(name);
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ClassT>::AddAssumeExists(const Name &name)
    {
        static_assert(std::is_pointer<T>::value, "The type given MUST be a pointer.");
        entryVector.push_back(entries.emplace(name, EntryPtr(new BasicAssumeExists<T, var>())).first);
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ClassT>::AddSimplePointer(const Name &name)
    {
        static_assert(std::is_pointer<T>::value, "The type given MUST be a pointer.");
        entryVector.push_back(entries.emplace(name, EntryPtr(new BasicSimplePointer<T, var>())).first);
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::AddLinkable(const Name &name)
    {
        AddLinkableImplementation<LinkableT<T>>(name);
    }

    template<class ClassT>
    template<class InterfaceT, class FileT>
    void TableBase<ClassT>::AddImposter(const Name &name)
    {
        AddImposterImplementation<LinkableT<InterfaceT>, LinkableT<FileT>>(name);
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::Link(const Name &name, T &arg)
    {
        FindEntry<LinkableBase<LinkableT<T>>>(name)->Link(RemoveConst(arg));
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::Link(const Name &name, T &&arg)
    {
        FindEntry<LinkableBase<LinkableT<T>>>(name)->LinkMove(std::move(arg));
    }

    template<class ClassT>
    template<class T>
    T& TableBase<ClassT>::Get(const Name &name) const
    {
        return FindEntry<EntryInherit<T>>(name)->Get();
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
        baseVector.push_back(bases.emplace(typeid(BaseT), BasePtr(new BaseDerived<BaseT>())).first);
    }

    template<class ClassT>
    void TableBase<ClassT>::Serialize(Scribe &scribe, ClassT &obj)
    {
        Reset();

        this->scribe = &scribe;

        for (auto &loop : baseVector)
            loop->second->Serialize(scribe, obj);

        for (auto &loop : entryVector)
            loop->second->Serialize(scribe, obj);
    }

    template<class ClassT>
    void TableBase<ClassT>::Construct(Scribe &scribe)
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
    TableBase<ClassT>::Entry::~Entry()
    {}

    template<class ClassT>
    template<class T>
    TableBase<ClassT>::LinkableBase<T>::~LinkableBase()
    {}

    template<class ClassT>
    TableBase<ClassT>::Base::~Base()
    {}

    template<class ClassT>
    template<class T, T ClassT::*var>
    typename TableBase<ClassT>::Basic<T, var>* TableBase<ClassT>::Basic<T, var>::Clone()
    {
        return new Basic();
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    T& TableBase<ClassT>::Basic<T, var>::Get()
    {
        return *const_cast<T*>(constructor->Get());
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ClassT>::Basic<T, var>::Serialize(Scribe &scribe, ClassT &obj)
    {
        scribe(RemoveConst(obj.*var));
    }
    
    template<class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ClassT>::Basic<T, var>::Construct(Scribe &scribe)
    {
        constructor.reset(new StackConstructor<StoreT>(scribe));
    }
    
    template<class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ClassT>::Basic<T, var>::Clear()
    {
        constructor.reset();
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    TableBase<ClassT>::BasicAssumeExists<T, var>::BasicAssumeExists() : obj(nullptr) {}

    template<class ClassT>
    template<class T, T ClassT::*var>
    typename TableBase<ClassT>::BasicAssumeExists<T, var>* TableBase<ClassT>::BasicAssumeExists<T, var>::Clone()
    {
        return new BasicAssumeExists();
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    T& TableBase<ClassT>::BasicAssumeExists<T, var>::Get()
    {
        return obj;
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ClassT>::BasicAssumeExists<T, var>::Serialize(Scribe &scribe, ClassT &obj)
    {
        scribe.AssumeExists(RemoveConst(obj.*var));
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ClassT>::BasicAssumeExists<T, var>::Construct(Scribe &scribe)
    {
        scribe.AssumeExistsLoad(obj);
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ClassT>::BasicAssumeExists<T, var>::Clear()
    {
        obj = nullptr;
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    TableBase<ClassT>::BasicSimplePointer<T, var>::BasicSimplePointer() : obj(nullptr)
    {}

    template<class ClassT>
    template<class T, T ClassT::*var>
    typename TableBase<ClassT>::BasicSimplePointer<T, var>* TableBase<ClassT>::BasicSimplePointer<T, var>::Clone()
    {
        return new BasicSimplePointer();
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    T& TableBase<ClassT>::BasicSimplePointer<T, var>::Get()
    {
        return obj;
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ClassT>::BasicSimplePointer<T, var>::Serialize(Scribe &scribe, ClassT &obj)
    {
        scribe.SimplePointer(RemoveConst(obj.*var));
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ClassT>::BasicSimplePointer<T, var>::Construct(Scribe &scribe)
    {
        scribe.SimplePointerLoad(obj);
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ClassT>::BasicSimplePointer<T, var>::Clear()
    {
        obj = nullptr;
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    typename TableBase<ClassT>::BasicInscripter<T, var>* TableBase<ClassT>::BasicInscripter<T, var>::Clone()
    {
        return new BasicInscripter();
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    T& TableBase<ClassT>::BasicInscripter<T, var>::Get()
    {
        return *const_cast<T*>(stored.get());
    }

    template<class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ClassT>::BasicInscripter<T, var>::Serialize(Scribe &scribe, ClassT &obj)
    {
        MakeAndSerializeTable<T>(scribe, (obj.*var));
    }
    
    template<class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ClassT>::BasicInscripter<T, var>::Construct(Scribe &scribe)
    {
        T *made = nullptr;
        ::inscription::Construct<T>(scribe, made);
        stored.reset(made);
    }
    
    template<class ClassT>
    template<class T, T ClassT::*var>
    void TableBase<ClassT>::BasicInscripter<T, var>::Clear()
    {
        stored.reset();
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::LinkableBase<T>::Serialize(Scribe &scribe, ClassT &obj)
    {
        if (scribe.IsOutput() && !IsValid())
            throw TableNotLinkedException();

        SerializeImpl(scribe, obj);
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::LinkableBase<T>::Construct(Scribe &scribe)
    {
        ConstructImpl(scribe);
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::LinkableBase<T>::Clear()
    {}

    template<class ClassT>
    template<class T>
    TableBase<ClassT>::LinkableForClass<T>::LinkableForClass() : var(nullptr) {}

    template<class ClassT>
    template<class T>
    typename TableBase<ClassT>::LinkableForClass<T>* TableBase<ClassT>::LinkableForClass<T>::Clone()
    {
        return new LinkableForClass(*this);
    }

    template<class ClassT>
    template<class T>
    T& TableBase<ClassT>::LinkableForClass<T>::Get()
    {
        return *var;
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::LinkableForClass<T>::Link(T &obj)
    {
        var = &obj;
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::LinkableForClass<T>::LinkMove(T &&obj)
    {
        var = &obj;
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::LinkableForClass<T>::SerializeImpl(Scribe &scribe, ClassT &obj)
    {
        scribe(*var);
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::LinkableForClass<T>::ConstructImpl(Scribe &scribe)
    {
        if (var)
            scribe(*var);
        else
            ::inscription::Construct(scribe, var);
    }

    template<class ClassT>
    template<class T>
    bool TableBase<ClassT>::LinkableForClass<T>::IsValid() const
    {
        return var != nullptr;
    }

    template<class ClassT>
    template<class T>
    TableBase<ClassT>::LinkableForNonClass<T>::LinkableForNonClass() : valid(false)
    {}

    template<class ClassT>
    template<class T>
    typename TableBase<ClassT>::LinkableForNonClass<T>* TableBase<ClassT>::LinkableForNonClass<T>::Clone()
    {
        return new LinkableForNonClass(*this);
    }

    template<class ClassT>
    template<class T>
    T& TableBase<ClassT>::LinkableForNonClass<T>::Get()
    {
        return var;
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::LinkableForNonClass<T>::Link(T &obj)
    {
        valid = true;
        var = obj;
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::LinkableForNonClass<T>::LinkMove(T &&obj)
    {
        valid = true;
        var = obj;
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::LinkableForNonClass<T>::SerializeImpl(Scribe &scribe, ClassT &obj)
    {
        scribe(var);
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::LinkableForNonClass<T>::ConstructImpl(Scribe &scribe)
    {
        valid = true;
        scribe(var);
    }

    template<class ClassT>
    template<class T>
    bool TableBase<ClassT>::LinkableForNonClass<T>::IsValid() const
    {
        return valid;
    }

    template<class ClassT>
    template<class InterfaceT, class FileT>
    TableBase<ClassT>::ImposterForClass<InterfaceT, FileT>::ImposterForClass() : deleteInterfaceVar(false)
    {}

    template<class ClassT>
    template<class InterfaceT, class FileT>
    TableBase<ClassT>::ImposterForClass<InterfaceT, FileT>::ImposterForClass(const ImposterForClass &arg) : deleteInterfaceVar(false)
    {}

    template<class ClassT>
    template<class InterfaceT, class FileT>
    TableBase<ClassT>::ImposterForClass<InterfaceT, FileT>::~ImposterForClass()
    {
        if (!deleteInterfaceVar)
            interfaceVar.release();
    }

    template<class ClassT>
    template<class InterfaceT, class FileT>
    typename TableBase<ClassT>::ImposterForClass<InterfaceT, FileT>* TableBase<ClassT>::ImposterForClass<InterfaceT, FileT>::Clone()
    {
        return new ImposterForClass(*this);
    }

    template<class ClassT>
    template<class InterfaceT, class FileT>
    InterfaceT& TableBase<ClassT>::ImposterForClass<InterfaceT, FileT>::Get()
    {
        return *interfaceVar;
    }

    template<class ClassT>
    template<class InterfaceT, class FileT>
    void TableBase<ClassT>::ImposterForClass<InterfaceT, FileT>::Link(InterfaceT &obj)
    {
        if (deleteInterfaceVar)
            interfaceVar.reset(&obj);
        else
        {
            interfaceVar.release();
            interfaceVar.reset(&obj);
        }

        deleteInterfaceVar = false;
    }

    template<class ClassT>
    template<class InterfaceT, class FileT>
    void TableBase<ClassT>::ImposterForClass<InterfaceT, FileT>::LinkMove(InterfaceT &&obj)
    {
        Link(obj);
    }

    template<class ClassT>
    template<class InterfaceT, class FileT>
    void TableBase<ClassT>::ImposterForClass<InterfaceT, FileT>::SerializeImpl(Scribe &scribe, ClassT &obj)
    {
        bool tracking = scribe.TrackObjects(false);

        // Save or load the fileT
        FileT &fileVar(::inscription::Convert<InterfaceT, FileT>(*interfaceVar));
        scribe(fileVar);
        // Set the interfaceT to the fileT
        Assign(*interfaceVar, fileVar);

        scribe.TrackObjects(tracking);
        scribe.TrackObject(interfaceVar.get());
    }

    template<class ClassT>
    template<class InterfaceT, class FileT>
    void TableBase<ClassT>::ImposterForClass<InterfaceT, FileT>::ConstructImpl(Scribe &scribe)
    {
        if (!deleteInterfaceVar)
            interfaceVar.release();

        bool tracking = scribe.TrackObjects(false);

        FileT *fileVar;
        ::inscription::Construct(scribe, fileVar);
        interfaceVar.reset(ConvertHeap<FileT, InterfaceT>(*fileVar));
        delete fileVar;

        scribe.TrackObjects(tracking);
        scribe.TrackObject(interfaceVar.get());

        deleteInterfaceVar = true;
    }

    template<class ClassT>
    template<class InterfaceT, class FileT>
    bool TableBase<ClassT>::ImposterForClass<InterfaceT, FileT>::IsValid() const
    {
        return interfaceVar != nullptr;
    }

    template<class ClassT>
    template<class InterfaceT, class FileT>
    TableBase<ClassT>::ImposterForNonClass<InterfaceT, FileT>::ImposterForNonClass() : valid(false)
    {}

    template<class ClassT>
    template<class InterfaceT, class FileT>
    typename TableBase<ClassT>::ImposterForNonClass<InterfaceT, FileT>* TableBase<ClassT>::ImposterForNonClass<InterfaceT, FileT>::Clone()
    {
        return new ImposterForNonClass(*this);
    }

    template<class ClassT>
    template<class InterfaceT, class FileT>
    InterfaceT& TableBase<ClassT>::ImposterForNonClass<InterfaceT, FileT>::Get()
    {
        return interfaceVar;
    }

    template<class ClassT>
    template<class InterfaceT, class FileT>
    void TableBase<ClassT>::ImposterForNonClass<InterfaceT, FileT>::Link(InterfaceT &obj)
    {
        valid = true;
        interfaceVar = obj;
    }

    template<class ClassT>
    template<class InterfaceT, class FileT>
    void TableBase<ClassT>::ImposterForNonClass<InterfaceT, FileT>::LinkMove(InterfaceT &&obj)
    {
        valid = true;
        interfaceVar = obj;
    }

    template<class ClassT>
    template<class InterfaceT, class FileT>
    void TableBase<ClassT>::ImposterForNonClass<InterfaceT, FileT>::SerializeImpl(Scribe &scribe, ClassT &obj)
    {
        FileT fileVar = interfaceVar;
        scribe(fileVar);
        interfaceVar = fileVar;
    }

    template<class ClassT>
    template<class InterfaceT, class FileT>
    void TableBase<ClassT>::ImposterForNonClass<InterfaceT, FileT>::ConstructImpl(Scribe &scribe)
    {
        valid = true;
        // 
        FileT fileVar;
        scribe(fileVar);
        interfaceVar = fileVar;
    }

    template<class ClassT>
    template<class InterfaceT, class FileT>
    bool TableBase<ClassT>::ImposterForNonClass<InterfaceT, FileT>::IsValid() const
    {
        return valid;
    }

    template<class ClassT>
    template<class T>
    typename TableBase<ClassT>::BaseDerived<T>* TableBase<ClassT>::BaseDerived<T>::Clone()
    {
        return new BaseDerived();
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::BaseDerived<T>::Serialize(Scribe &scribe, ClassT &obj)
    {
        MakeAndSerializeTable<T>(scribe, obj);
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::BaseDerived<T>::Construct(Scribe &scribe)
    {
        base = MakeAndConstructTable<T>(scribe);
    }

    template<class ClassT>
    template<class T, T ClassT::*var, typename std::enable_if<!Access::InscripterT<T>::exists, int>::type>
    void TableBase<ClassT>::AddImplementation(const Name &name)
    {
        AddEntryGeneral(name, new Basic<T, var>());
    }

    template<class ClassT>
    template<class T, T ClassT::*var, typename std::enable_if<Access::InscripterT<T>::exists, int>::type>
    void TableBase<ClassT>::AddImplementation(const Name &name)
    {
        AddEntryGeneral(name, new BasicInscripter<T, var>());
    }

    template<class ClassT>
    template<class T, typename std::enable_if<std::is_class<T>::value, int>::type>
    void TableBase<ClassT>::AddLinkableImplementation(const Name &name)
    {
        AddEntryGeneral(name, new LinkableForClass<T>());
    }

    template<class ClassT>
    template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type>
    void TableBase<ClassT>::AddLinkableImplementation(const Name &name)
    {
        AddEntryGeneral(name, new LinkableForNonClass<T>());
    }

    template<class ClassT>
    template<class InterfaceT, class RealT, typename std::enable_if<std::is_class<RealT>::value, int>::type>
    void TableBase<ClassT>::AddImposterImplementation(const Name &name)
    {
        AddEntryGeneral(name, new ImposterForClass<InterfaceT, RealT>());
    }

    template<class ClassT>
    template<class InterfaceT, class RealT, typename std::enable_if<!std::is_class<RealT>::value, int>::type>
    void TableBase<ClassT>::AddImposterImplementation(const Name &name)
    {
        AddEntryGeneral(name, new ImposterForNonClass<InterfaceT, RealT>());
    }

    template<class ClassT>
    template<class T>
    void TableBase<ClassT>::AddEntryGeneral(const Name &name, T *add)
    {
        entryVector.push_back(entries.emplace(name, EntryPtr(add)).first);
    }

    template<class ClassT>
    template<class T>
    T* TableBase<ClassT>::FindEntry(const Name &name) const
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
    typename TableBase<ClassT>::BaseDerived<BaseT>* TableBase<ClassT>::FindBase() const
    {
        auto found = bases.find(typeid(BaseT));
        if (found == bases.end())
            throw TableVariableNotHereException();

#ifndef INSCRIPTION_DEBUG
        return static_cast<BaseDerived<BaseT>*>(found->second.get());
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
        Table(const Table &arg);
        Table& operator=(const Table &arg);
        Table(Table &&arg);
        Table& operator=(Table &&arg);
    };

    template<class ClassT>
    Table<ClassT>::Table(const Table &arg) : TableBase(arg)
    {}

    template<class ClassT>
    Table<ClassT>& Table<ClassT>::operator=(const Table &arg)
    {
        TableBase::operator=(arg);
        return *this;
    }

    template<class ClassT>
    Table<ClassT>::Table(Table &&arg) : TableBase(std::move(arg))
    {}

    template<class ClassT>
    Table<ClassT>& Table<ClassT>::operator=(Table &&arg)
    {
        TableBase::operator=(std::move(arg));
        return *this;
    }
}

#include "UndefAssert.h"