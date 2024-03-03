#pragma once

#include "Table.h"

namespace Inscription
{
    class Scribe;
    class ClassNameContainer;

    template<class T>
    class InscripterBase
    {
    public:
        static constexpr bool exists = true;
        typedef T ManagedT;
        typedef ::Inscription::Table<ManagedT> TableT;
    public:
        static void SerializeTable(TableT &table, Scribe &scribe, ManagedT &obj);
        static void ConstructTable(TableT &table, Scribe &scribe);
        static void ConstructObjectInstance(Scribe &scribe, ManagedT *&obj);
        static void PostConstruct(ManagedT &obj);
    private:
        InscripterBase(const InscripterBase &arg) = delete;
        InscripterBase& operator=(const InscripterBase &arg) = delete;
    };

    template<class T>
    void InscripterBase<T>::SerializeTable(TableT &table, Scribe &scribe, ManagedT &obj)
    {
        table.Serialize(scribe, obj);
    }

    template<class T>
    void InscripterBase<T>::ConstructTable(TableT &table, Scribe &scribe)
    {
        table.Construct(scribe);
    }

    template<class T>
    void InscripterBase<T>::ConstructObjectInstance(Scribe &scribe, ManagedT *&obj)
    {
        obj = new ManagedT(MakeAndConstructTable<ManagedT>(scribe));
    }

    template<class T>
    void InscripterBase<T>::PostConstruct(ManagedT &obj)
    {}

    template<class T>
    typename InscripterBase<T>::TableT MakeAndSerializeTable(Scribe &scribe, typename InscripterBase<T>::ManagedT &obj)
    {
        typedef Access::InscripterT<T> InscripterT;
        typedef typename InscripterT::TableT TableT;

        TableT table = InscripterT::CreateTable(scribe);
        InscripterT::SerializeTable(table, scribe, obj);
        return table;
    }

    template<class T>
    typename InscripterBase<T>::TableT MakeAndConstructTable(Scribe &scribe)
    {
        typedef Access::InscripterT<T> InscripterT;
        typedef typename InscripterT::TableT TableT;

        TableT table = InscripterT::CreateTable(scribe);
        InscripterT::ConstructTable(table, scribe);
        return table;
    }
}