#pragma once

#include "Scribe.h"
#include "TableData.h"

namespace Inscription
{
    template<class Base, class Object, class Format>
    class BaseTableDataLink
    {
    public:
        using BaseT = Base;
        using ObjectT = Object;
        using FormatT = Format;
    private:
        using BaseScribe = Scribe<Base>;
    public:
        using TableT = typename BaseScribe::Table;
        using DataT = TableData<BaseT, FormatT>;
    public:
        BaseTableDataLink() = default;

        BaseTableDataLink(const BaseTableDataLink& arg);
        BaseTableDataLink(BaseTableDataLink&& arg) noexcept;

        BaseTableDataLink& operator=(const BaseTableDataLink& arg);
        BaseTableDataLink& operator=(BaseTableDataLink&& arg) noexcept;

        void Scriven(FormatT& format);
        void ObjectScriven(ObjectT& object, FormatT& format);

        void PullFromObject(ObjectT& object, FormatT& format);
        void PushToObject(ObjectT& object, FormatT& format);
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

    template<class Base, class Object, class Format>
    BaseTableDataLink<Base, Object, Format>::BaseTableDataLink(const BaseTableDataLink& arg) :
        baseTable(arg.baseTable)
    {}

    template<class Base, class Object, class Format>
    BaseTableDataLink<Base, Object, Format>::BaseTableDataLink(BaseTableDataLink&& arg) noexcept :
        baseTable(std::move(arg.baseTable))
    {}

    template<class Base, class Object, class Format>
    BaseTableDataLink<Base, Object, Format>& BaseTableDataLink<Base, Object, Format>::operator=(
        const BaseTableDataLink& arg)
    {
        baseTable = arg.baseTable;
        return *this;
    }

    template<class Base, class Object, class Format>
    BaseTableDataLink<Base, Object, Format>& BaseTableDataLink<Base, Object, Format>::operator=(
        BaseTableDataLink&& arg) noexcept
    {
        baseTable = std::move(arg.baseTable);
        return *this;
    }

    template<class Base, class Object, class Format>
    void BaseTableDataLink<Base, Object, Format>::Scriven(FormatT& format)
    {
        baseTable.Scriven(format);
    }

    template<class Base, class Object, class Format>
    void BaseTableDataLink<Base, Object, Format>::ObjectScriven(ObjectT& object, FormatT& format)
    {
        baseTable.ObjectScriven(object, format);
    }

    template<class Base, class Object, class Format>
    void BaseTableDataLink<Base, Object, Format>::PullFromObject(ObjectT& object, FormatT& format)
    {
        baseTable.PullFromObject(object, format);
    }

    template<class Base, class Object, class Format>
    void BaseTableDataLink<Base, Object, Format>::PushToObject(ObjectT& object, FormatT& format)
    {
        baseTable.PushToObject(object, format);
    }

    template<class Base, class Object, class Format>
    BaseTableDataLink<Base, Object, Format>::operator DataT&()
    {
        return Data();
    }

    template<class Base, class Object, class Format>
    BaseTableDataLink<Base, Object, Format>::operator const DataT&() const
    {
        return Data();
    }

    template<class Base, class Object, class Format>
    auto BaseTableDataLink<Base, Object, Format>::operator*() -> DataT&
    {
        return Data();
    }

    template<class Base, class Object, class Format>
    auto BaseTableDataLink<Base, Object, Format>::operator*() const -> const DataT&
    {
        return Data();
    }

    template<class Base, class Object, class Format>
    auto BaseTableDataLink<Base, Object, Format>::operator->() -> DataT*
    {
        return &Data();
    }

    template<class Base, class Object, class Format>
    auto BaseTableDataLink<Base, Object, Format>::operator->() const -> const DataT*
    {
        return &Data();
    }

    template<class Base, class Object, class Format>
    auto BaseTableDataLink<Base, Object, Format>::Data() -> DataT&
    {
        return baseTable.data;
    }

    template<class Base, class Object, class Format>
    auto BaseTableDataLink<Base, Object, Format>::Data() const -> const DataT&
    {
        return baseTable.data;
    }

    template<class Base, class Object, class Format>
    auto BaseTableDataLink<Base, Object, Format>::Table() -> TableT&
    {
        return baseTable;
    }

    template<class Base, class Object, class Format>
    auto BaseTableDataLink<Base, Object, Format>::Table() const -> const TableT&
    {
        return baseTable;
    }
}