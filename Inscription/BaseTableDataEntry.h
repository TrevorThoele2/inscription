#pragma once

#include <typeindex>

#include "Scribe.h"
#include "TableData.h"

namespace Inscription
{
    template<class Base, class Object, class Archive>
    class BaseTableDataEntry
    {
    public:
        using BaseT = Base;
        using ObjectT = Object;
        using ArchiveT = Archive;
    private:
        using BaseScribe = typename Scribe<Base, ArchiveT>;
    public:
        using TableT = typename BaseScribe::Table;
        using DataT = typename TableData<BaseT, ArchiveT>;
    public:
        BaseTableDataEntry();

        BaseTableDataEntry(const BaseTableDataEntry& arg);
        BaseTableDataEntry(BaseTableDataEntry&& arg);

        BaseTableDataEntry& operator=(const BaseTableDataEntry& arg);
        BaseTableDataEntry& operator=(BaseTableDataEntry&& arg);

        void Scriven(ArchiveT& archive);
        void ObjectScriven(ObjectT& object, ArchiveT& archive);

        void PullFromObject(ObjectT& object, ArchiveT& archive);
        void PushToObject(ObjectT& object, ArchiveT& archive);
    public:
        operator DataT&();
        operator const DataT&() const;

        DataT& operator*();
        const DataT& operator*() const;

        DataT* operator->();
        const DataT* operator->() const;

        DataT& Data();
        const DataT& Data() const;

        TableT& Table();
        const TableT& Table() const;
    public:
        TableT baseTable;
    };

    template<class Base, class Object, class Archive>
    BaseTableDataEntry<Base, Object, Archive>::BaseTableDataEntry() 
    {}

    template<class Base, class Object, class Archive>
    BaseTableDataEntry<Base, Object, Archive>::BaseTableDataEntry(const BaseTableDataEntry& arg) :
        baseTable(arg.baseTable)
    {}

    template<class Base, class Object, class Archive>
    BaseTableDataEntry<Base, Object, Archive>::BaseTableDataEntry(BaseTableDataEntry&& arg) :
        baseTable(std::move(arg.baseTable))
    {}

    template<class Base, class Object, class Archive>
    BaseTableDataEntry<Base, Object, Archive>& BaseTableDataEntry<Base, Object, Archive>::operator=(
        const BaseTableDataEntry& arg)
    {
        baseTable = arg.baseTable;
        return *this;
    }

    template<class Base, class Object, class Archive>
    BaseTableDataEntry<Base, Object, Archive>& BaseTableDataEntry<Base, Object, Archive>::operator=(
        BaseTableDataEntry&& arg)
    {
        baseTable = std::move(arg.baseTable);
        return *this;
    }

    template<class Base, class Object, class Archive>
    void BaseTableDataEntry<Base, Object, Archive>::Scriven(ArchiveT& archive)
    {
        baseTable.Scriven(archive);
    }

    template<class Base, class Object, class Archive>
    void BaseTableDataEntry<Base, Object, Archive>::ObjectScriven(ObjectT& object, ArchiveT& archive)
    {
        baseTable.ObjectScriven(object, archive);
    }

    template<class Base, class Object, class Archive>
    void BaseTableDataEntry<Base, Object, Archive>::PullFromObject(ObjectT& object, ArchiveT& archive)
    {
        baseTable.PullFromObject(object, archive);
    }

    template<class Base, class Object, class Archive>
    void BaseTableDataEntry<Base, Object, Archive>::PushToObject(ObjectT& object, ArchiveT& archive)
    {
        baseTable.PushToObject(object, archive);
    }

    template<class Base, class Object, class Archive>
    BaseTableDataEntry<Base, Object, Archive>::operator DataT&()
    {
        return Data();
    }

    template<class Base, class Object, class Archive>
    BaseTableDataEntry<Base, Object, Archive>::operator const DataT&() const
    {
        return Data();
    }

    template<class Base, class Object, class Archive>
    auto BaseTableDataEntry<Base, Object, Archive>::operator*() -> DataT&
    {
        return Data();
    }

    template<class Base, class Object, class Archive>
    auto BaseTableDataEntry<Base, Object, Archive>::operator*() const -> const DataT&
    {
        return Data();
    }

    template<class Base, class Object, class Archive>
    auto BaseTableDataEntry<Base, Object, Archive>::operator->() -> DataT*
    {
        return &Data();
    }

    template<class Base, class Object, class Archive>
    auto BaseTableDataEntry<Base, Object, Archive>::operator->() const -> const DataT*
    {
        return &Data();
    }

    template<class Base, class Object, class Archive>
    auto BaseTableDataEntry<Base, Object, Archive>::Data() -> DataT&
    {
        return baseTable.data;
    }

    template<class Base, class Object, class Archive>
    auto BaseTableDataEntry<Base, Object, Archive>::Data() const -> const DataT&
    {
        return baseTable.data;
    }

    template<class Base, class Object, class Archive>
    auto BaseTableDataEntry<Base, Object, Archive>::Table() -> TableT&
    {
        return baseTable;
    }

    template<class Base, class Object, class Archive>
    auto BaseTableDataEntry<Base, Object, Archive>::Table() const -> const TableT&
    {
        return baseTable;
    }
}