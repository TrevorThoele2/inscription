#pragma once

#include "Access.h"
#include "Table.h"

namespace Inscription
{
    class Scribe;
    class ClassNameResolver;

    template<class T>
    class Inscripter;

    template<class T>
    class InscripterBase
    {
    public:
        static constexpr bool exists = true;
        typedef T ManagedT;
        typedef ::Inscription::Table<ManagedT> TableT;
    public:
        static void SerializeTable(TableT& table, Scribe& scribe, ManagedT& obj);
        static void ConstructTable(TableT& table, Scribe& scribe);
        static void ConstructObjectInstance(Scribe& scribe, ManagedT*& obj);
        static void PostConstruct(ManagedT& obj);
    private:
        InscripterBase(const InscripterBase& arg) = delete;
        InscripterBase& operator=(const InscripterBase& arg) = delete;
    private:
        template<class U, typename std::enable_if<std::is_abstract<U>::value, int>::type = 0>
        static void ConstructObjectInstanceImpl(Scribe& scribe, ManagedT*& obj)
        {
            obj = nullptr;
        }

        template<class U, typename std::enable_if<!std::is_abstract<U>::value && std::is_constructible<U, const TableT&>::value, int>::type = 0>
        static void ConstructObjectInstanceImpl(Scribe& scribe, ManagedT*& obj)
        {
            obj = new ManagedT(MakeAndConstructTable<ManagedT>(scribe));
        }

        template<class U, typename std::enable_if<!std::is_abstract<U>::value && !std::is_constructible<U, const TableT&>::value, int>::type = 0>
        static void ConstructObjectInstanceImpl(Scribe& scribe, ManagedT*& obj)
        {
            static_assert(IsDefaultConstructible<T>::value, "Non-default constructible objects must have an inscripter to be constructed.");

            obj = new ManagedT();
            MakeAndSerializeTable<T>(scribe, *obj);
        }
    };

    template<class T>
    void InscripterBase<T>::SerializeTable(TableT& table, Scribe& scribe, ManagedT& obj)
    {
        table.Serialize(scribe, obj);
    }

    template<class T>
    void InscripterBase<T>::ConstructTable(TableT& table, Scribe& scribe)
    {
        table.Construct(scribe);
    }

    template<class T>
    void InscripterBase<T>::ConstructObjectInstance(Scribe& scribe, ManagedT*& obj)
    {
        ConstructObjectInstanceImpl<T>(scribe, obj);
    }

    template<class T>
    void InscripterBase<T>::PostConstruct(ManagedT& obj)
    {}

    template<class T>
    typename InscripterBase<T>::TableT MakeAndSerializeTable(Scribe& scribe, typename InscripterBase<T>::ManagedT& obj)
    {
        typedef Access::InscripterT<T> InscripterT;
        typedef typename InscripterT::TableT TableT;

        TableT table = InscripterT::CreateTable(scribe);
        InscripterT::SerializeTable(table, scribe, obj);
        return table;
    }

    template<class T>
    typename InscripterBase<T>::TableT MakeAndConstructTable(Scribe& scribe)
    {
        typedef Access::InscripterT<T> InscripterT;
        typedef typename InscripterT::TableT TableT;

        TableT table = InscripterT::CreateTable(scribe);
        InscripterT::ConstructTable(table, scribe);
        return table;
    }
}