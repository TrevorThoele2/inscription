#pragma once

#include "TableData.h"
#include "AutoTableDataEntry.h"

namespace Inscription
{
    template<class Object, class Archive>
    class TableBase
    {
    public:
        using ObjectT = Object;
        using ArchiveT = Archive;

        using DataT = TableData<ObjectT, ArchiveT>;
    public:
        virtual void Scriven(DataT& data, ArchiveT& archive);
        virtual void Construct(DataT& data, ObjectT*& object, ArchiveT& archive) = 0;

        virtual void PushToObject(DataT& data, ObjectT& object);
        virtual void PullFromObject(DataT& data, ObjectT& object);
    protected:
        using DataEntry = AutoTableDataEntry<ObjectT, ArchiveT>;
        using DataEntryList = std::vector<DataEntry>;
        void MergeDataEntries(DataEntryList&& entries);
    private:
        DataEntryList dataEntryList;
    };

    template<class Object, class Archive>
    void TableBase<Object, Archive>::Scriven(DataT& data, ArchiveT& archive)
    {
        for (auto& loop : dataEntryList)
            loop.Scriven(data, archive);
    }

    template<class Object, class Archive>
    void TableBase<Object, Archive>::PullFromObject(DataT& data, ObjectT& object)
    {
        for (auto& loop : dataEntryList)
            loop.PullFromObject(data, object);
    }

    template<class Object, class Archive>
    void TableBase<Object, Archive>::PushToObject(DataT& data, ObjectT& object)
    {
        for (auto& loop : dataEntryList)
            loop.PushToObject(data, object);
    }

    template<class Object, class Archive>
    void TableBase<Object, Archive>::MergeDataEntries(DataEntryList&& entries)
    {
        dataEntryList.insert(
            dataEntryList.end(),
            std::make_move_iterator(entries.begin()),
            std::make_move_iterator(entries.end()));
        entries.clear();
    }
}