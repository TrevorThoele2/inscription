#pragma once

#include "Scribe.h"
#include "TableData.h"

namespace Inscription
{
    template<class Base, class Object, class Archive>
    class BaseTableDataLink
    {
    public:
        using BaseT = Base;
        using ObjectT = Object;
        using ArchiveT = Archive;
    private:
        using BaseScribe = Scribe<Base, ArchiveT>;
    public:
        using TableT = typename BaseScribe::Table;
        using DataT = TableData<BaseT, ArchiveT>;
    public:
        BaseTableDataLink() = default;

        BaseTableDataLink(const BaseTableDataLink& arg);
        BaseTableDataLink(BaseTableDataLink&& arg) noexcept;

        BaseTableDataLink& operator=(const BaseTableDataLink& arg);
        BaseTableDataLink& operator=(BaseTableDataLink&& arg) noexcept;

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

        [[nodiscard]] DataT& Data();
        [[nodiscard]] const DataT& Data() const;

        [[nodiscard]] TableT& Table();
        [[nodiscard]] const TableT& Table() const;
    public:
        TableT baseTable;
    };

    template<class Base, class Object, class Archive>
    BaseTableDataLink<Base, Object, Archive>::BaseTableDataLink(const BaseTableDataLink& arg) :
        baseTable(arg.baseTable)
    {}

    template<class Base, class Object, class Archive>
    BaseTableDataLink<Base, Object, Archive>::BaseTableDataLink(BaseTableDataLink&& arg) noexcept :
        baseTable(std::move(arg.baseTable))
    {}

    template<class Base, class Object, class Archive>
    BaseTableDataLink<Base, Object, Archive>& BaseTableDataLink<Base, Object, Archive>::operator=(
        const BaseTableDataLink& arg)
    {
        baseTable = arg.baseTable;
        return *this;
    }

    template<class Base, class Object, class Archive>
    BaseTableDataLink<Base, Object, Archive>& BaseTableDataLink<Base, Object, Archive>::operator=(
        BaseTableDataLink&& arg) noexcept
    {
        baseTable = std::move(arg.baseTable);
        return *this;
    }

    template<class Base, class Object, class Archive>
    void BaseTableDataLink<Base, Object, Archive>::Scriven(ArchiveT& archive)
    {
        baseTable.Scriven(archive);
    }

    template<class Base, class Object, class Archive>
    void BaseTableDataLink<Base, Object, Archive>::ObjectScriven(ObjectT& object, ArchiveT& archive)
    {
        baseTable.ObjectScriven(object, archive);
    }

    template<class Base, class Object, class Archive>
    void BaseTableDataLink<Base, Object, Archive>::PullFromObject(ObjectT& object, ArchiveT& archive)
    {
        baseTable.PullFromObject(object, archive);
    }

    template<class Base, class Object, class Archive>
    void BaseTableDataLink<Base, Object, Archive>::PushToObject(ObjectT& object, ArchiveT& archive)
    {
        baseTable.PushToObject(object, archive);
    }

    template<class Base, class Object, class Archive>
    BaseTableDataLink<Base, Object, Archive>::operator DataT&()
    {
        return Data();
    }

    template<class Base, class Object, class Archive>
    BaseTableDataLink<Base, Object, Archive>::operator const DataT&() const
    {
        return Data();
    }

    template<class Base, class Object, class Archive>
    auto BaseTableDataLink<Base, Object, Archive>::operator*() -> DataT&
    {
        return Data();
    }

    template<class Base, class Object, class Archive>
    auto BaseTableDataLink<Base, Object, Archive>::operator*() const -> const DataT&
    {
        return Data();
    }

    template<class Base, class Object, class Archive>
    auto BaseTableDataLink<Base, Object, Archive>::operator->() -> DataT*
    {
        return &Data();
    }

    template<class Base, class Object, class Archive>
    auto BaseTableDataLink<Base, Object, Archive>::operator->() const -> const DataT*
    {
        return &Data();
    }

    template<class Base, class Object, class Archive>
    auto BaseTableDataLink<Base, Object, Archive>::Data() -> DataT&
    {
        return baseTable.data;
    }

    template<class Base, class Object, class Archive>
    auto BaseTableDataLink<Base, Object, Archive>::Data() const -> const DataT&
    {
        return baseTable.data;
    }

    template<class Base, class Object, class Archive>
    auto BaseTableDataLink<Base, Object, Archive>::Table() -> TableT&
    {
        return baseTable;
    }

    template<class Base, class Object, class Archive>
    auto BaseTableDataLink<Base, Object, Archive>::Table() const -> const TableT&
    {
        return baseTable;
    }
}