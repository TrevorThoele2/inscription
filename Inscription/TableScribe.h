#pragma once

#include "CompositeScribe.h"

#include "Table.h"
#include "TableBase.h"

namespace Inscription
{
    template<class Object, class Archive>
    class TableScribe : public CompositeScribe<Object, Archive>
    {
    private:
        using BaseT = typename CompositeScribe<Object, Archive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        using TableT = Table<ObjectT, ArchiveT>;
    public:
        using ClassNameResolver = typename BaseT::ClassNameResolver;
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
        static void Construct(ObjectT*& object, ArchiveT& archive);
    private:
        TableScribe() = delete;
        TableScribe(const TableScribe& arg) = delete;
        TableScribe& operator=(const TableScribe& arg) = delete;
    private:
        static_assert(std::is_class_v<ObjectT>,
            "The Object given to a TableScribe was not a composite.");
    };

    template<class Object, class Archive>
    void TableScribe<Object, Archive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        auto table = TableT();
        auto tableData = typename TableT::DataT();
        if (archive.IsOutput())
        {
            table.PullFromObject(tableData, object);
            table.Scriven(tableData, archive);
        }
        else
        {
            table.Scriven(tableData, archive);
            table.PushToObject(tableData, object);
        }
    }

    template<class Object, class Archive>
    void TableScribe<Object, Archive>::Construct(ObjectT*& object, ArchiveT& archive)
    {
        auto table = TableT();
        auto tableData = typename TableT::DataT();
        table.Scriven(tableData, archive);
        table.Construct(tableData, object, archive);
    }
}