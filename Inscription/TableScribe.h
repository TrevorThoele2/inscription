#pragma once

#include "CompositeScribe.h"

#include "TableBase.h"
#include "AutoTableDataEntry.h"
#include "BaseTableDataEntry.h"
#include "NullTableData.h"

namespace Inscription
{
    template<class Object, class Archive>
    class TableScribe : public CompositeScribe<Object, Archive>
    {
    private:
        using BaseScribeT = typename CompositeScribe<Object, Archive>;
    public:
        using ObjectT = typename BaseScribeT::ObjectT;
        using ArchiveT = typename BaseScribeT::ArchiveT;
        using ClassNameResolver = typename BaseScribeT::ClassNameResolver;
    public:
        template<class T>
        using ComposingScribe = typename BaseScribeT::template ComposingScribe<T>;
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
        static void Construct(ObjectT* storage, ArchiveT& archive);
    public:
        using TableBase = TableBase<ObjectT, ArchiveT>;
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
        using TableT = typename Scribe<ObjectT, ArchiveT>::Table;

        auto table = TableT();
        if (archive.IsOutput())
        {
            table.PullFromObject(object);
            table.Scriven(archive);
            table.ObjectScriven(object, archive);
        }
        else
        {
            table.Scriven(archive);
            table.PushToObject(object);
            table.ObjectScriven(object, archive);
        }
    }

    template<class Object, class Archive>
    void TableScribe<Object, Archive>::Construct(ObjectT* storage, ArchiveT& archive)
    {
        using TableT = typename Scribe<ObjectT, ArchiveT>::Table;

        auto table = TableT();
        table.Scriven(archive);
        table.Construct(storage, archive);
        table.PushToObject(*storage);
        table.ObjectScriven(*storage, archive);
    }
}

#include "UndefAssert.h"