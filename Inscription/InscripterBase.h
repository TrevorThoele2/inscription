#pragma once

#include "Access.h"
#include "Table.h"

namespace Inscription
{
    template<class T>
    class Inscripter;

    template<class T>
    class InscripterBase
    {
    public:
        static constexpr bool exists = true;
        typedef T ManagedT;
        template<class ScribeT>
        using Table = ::Inscription::Table<ScribeT, ManagedT>;
    public:
        template<class ScribeT>
        static void SerializeTable(Table<ScribeT>& table, ScribeT& scribe, ManagedT& obj);
        template<class ScribeT>
        static void ConstructTable(Table<ScribeT>& table, ScribeT& scribe);
        template<class ScribeT>
        static void ConstructObjectInstance(ScribeT& scribe, ManagedT*& obj);
        static void PostConstruct(ManagedT& obj);
    private:
        InscripterBase(const InscripterBase& arg) = delete;
        InscripterBase& operator=(const InscripterBase& arg) = delete;
    private:
        template<
            class U,
            class ScribeT,
            typename std::enable_if<std::is_abstract<U>::value, int>::type = 0>
        static void ConstructObjectInstanceImpl(ScribeT& scribe, ManagedT*& obj)
        {
            obj = nullptr;
        }

        template<
            class U,
            class ScribeT,
            typename std::enable_if<!std::is_abstract<U>::value && std::is_constructible<U, const Table<ScribeT>&>::value, int>::type = 0>
        static void ConstructObjectInstanceImpl(ScribeT& scribe, ManagedT*& obj)
        {
            obj = new ManagedT(MakeAndConstructTable<ManagedT>(scribe));
        }

        template<
            class U,
            class ScribeT,
            typename std::enable_if<!std::is_abstract<U>::value && !std::is_constructible<U, const Table<ScribeT>&>::value, int>::type = 0>
        static void ConstructObjectInstanceImpl(ScribeT& scribe, ManagedT*& obj)
        {
            static_assert(IsDefaultConstructible<T>::value, "Non-default constructible objects must have an inscripter to be constructed.");

            obj = new ManagedT();
            MakeAndSerializeTable<T>(scribe, *obj);
        }
    };

    template<class T>
    template<class ScribeT>
    void InscripterBase<T>::SerializeTable(Table<ScribeT>& table, ScribeT& scribe, ManagedT& obj)
    {
        table.Serialize(scribe, obj);
    }

    template<class T>
    template<class ScribeT>
    void InscripterBase<T>::ConstructTable(Table<ScribeT>& table, ScribeT& scribe)
    {
        table.Construct(scribe);
    }

    template<class T>
    template<class ScribeT>
    void InscripterBase<T>::ConstructObjectInstance(ScribeT& scribe, ManagedT*& obj)
    {
        ConstructObjectInstanceImpl<T>(scribe, obj);
    }

    template<class T>
    void InscripterBase<T>::PostConstruct(ManagedT& obj)
    {}

    template<class T, class ScribeT>
    typename InscripterBase<T>::template Table<ScribeT> MakeAndSerializeTable(ScribeT& scribe, typename InscripterBase<T>::ManagedT& obj)
    {
        typedef Access::InscripterT<T> InscripterT;
        typedef typename InscripterT::template Table<ScribeT> TableT;

        TableT table = InscripterT::CreateTable(scribe);
        InscripterT::SerializeTable(table, scribe, obj);
        return table;
    }

    template<class T, class ScribeT>
    typename InscripterBase<T>::template Table<ScribeT> MakeAndConstructTable(ScribeT& scribe)
    {
        typedef Access::InscripterT<T> InscripterT;
        typedef typename InscripterT::template Table<ScribeT> TableT;

        TableT table = InscripterT::CreateTable(scribe);
        InscripterT::ConstructTable(table, scribe);
        return table;
    }
}