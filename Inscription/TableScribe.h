#pragma once

#include "CompositeScribe.h"

#include "AutoTableDataEntry.h"

#include "Assert.h"

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
        using ClassNameResolver = typename BaseT::ClassNameResolver;
    public:
        template<class T>
        using ComposingScribe = typename BaseT::template ComposingScribe<T>;
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
        static void Construct(ObjectT*& object, ArchiveT& archive);
    public:
        template<class Data>
        class TableBase
        {
        public:
            using ObjectT = Object;
            using ArchiveT = Archive;
        public:
            using DataT = Data;
        public:
            void Scriven(DataT& data, ArchiveT& archive);
            void ObjectScriven(DataT& data, ObjectT& object, ArchiveT& archive);
            void Construct(DataT& data, ObjectT*& object, ArchiveT& archive);

            void PushToObject(DataT& data, ObjectT& object);
            void PullFromObject(DataT& data, ObjectT& object);
        protected:
            virtual void ScrivenImplementation(DataT& data, ArchiveT& archive);
            virtual void ObjectScrivenImplementation(DataT& data, ObjectT& object, ArchiveT& archive);
            virtual void ConstructImplementation(DataT& data, ObjectT*& object, ArchiveT& archive);
            virtual void PushToObjectImplementation(DataT& data, ObjectT& object);
            virtual void PullFromObjectImplementation(DataT& data, ObjectT& object);
        protected:
            using DataEntry = AutoTableDataEntry<DataT, ObjectT, ArchiveT>;
            using DataEntryList = std::vector<DataEntry>;
            void AddDataEntry(DataEntry&& entry);
            void MergeDataEntries(DataEntryList&& entries);

            template<class BaseT>
            typename ComposingScribe<BaseT>::Table::DataT* AsAutoBase()
            {
                for (auto& loop : dataEntryList)
                {
                    auto baseData = loop.AsBase<BaseT>();
                    if (baseData)
                        return baseData;
                }

                return nullptr;
            }
        private:
            DataEntryList dataEntryList;
        private:
            void DoConstruct(DataT& data, ObjectT*& object, ArchiveT& archive, std::true_type);
            void DoConstruct(DataT& data, ObjectT*& object, ArchiveT& archive, std::false_type);
        };
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
        auto tableData = typename TableT::DataT();
        if (archive.IsOutput())
        {
            table.PullFromObject(tableData, object);
            table.Scriven(tableData, archive);
            table.ObjectScriven(tableData, object, archive);
        }
        else
        {
            table.Scriven(tableData, archive);
            table.PushToObject(tableData, object);
            table.ObjectScriven(tableData, object, archive);
        }
    }

    template<class Object, class Archive>
    void TableScribe<Object, Archive>::Construct(ObjectT*& object, ArchiveT& archive)
    {
        using TableT = typename Scribe<ObjectT, ArchiveT>::Table;

        auto table = TableT();
        auto tableData = typename TableT::DataT();
        table.Scriven(tableData, archive);
        table.Construct(tableData, object, archive);
        table.ObjectScriven(tableData, *object, archive);
    }

    template<class Object, class Archive>
    template<class Data>
    void TableScribe<Object, Archive>::TableBase<Data>::Scriven(DataT& data, ArchiveT& archive)
    {
        for (auto& loop : dataEntryList)
            loop.Scriven(data, archive);

        ScrivenImplementation(data, archive);
    }

    template<class Object, class Archive>
    template<class Data>
    void TableScribe<Object, Archive>::TableBase<Data>::ObjectScriven(DataT& data, ObjectT& object, ArchiveT& archive)
    {
        for (auto& loop : dataEntryList)
            loop.ObjectScriven(data, object, archive);

        ObjectScrivenImplementation(data, object, archive);
    }

    template<class Object, class Archive>
    template<class Data>
    void TableScribe<Object, Archive>::TableBase<Data>::Construct(DataT& data, ObjectT*& object, ArchiveT& archive)
    {
        ConstructImplementation(data, object, archive);
    }

    template<class Object, class Archive>
    template<class Data>
    void TableScribe<Object, Archive>::TableBase<Data>::PushToObject(DataT& data, ObjectT& object)
    {
        for (auto& loop : dataEntryList)
            loop.PushToObject(data, object);

        PushToObjectImplementation(data, object);
    }

    template<class Object, class Archive>
    template<class Data>
    void TableScribe<Object, Archive>::TableBase<Data>::PullFromObject(DataT& data, ObjectT& object)
    {
        for (auto& loop : dataEntryList)
            loop.PullFromObject(data, object);

        PullFromObjectImplementation(data, object);
    }

    template<class Object, class Archive>
    template<class Data>
    void TableScribe<Object, Archive>::TableBase<Data>::ScrivenImplementation(
        DataT& data, ArchiveT& archive)
    {}

    template<class Object, class Archive>
    template<class Data>
    void TableScribe<Object, Archive>::TableBase<Data>::ObjectScrivenImplementation(
        DataT& data, ObjectT& object, ArchiveT& archive)
    {}

    template<class Object, class Archive>
    template<class Data>
    void TableScribe<Object, Archive>::TableBase<Data>::ConstructImplementation(
        DataT& data, ObjectT*& object, ArchiveT& archive)
    {
        DoConstruct(
            data,
            object,
            archive,
            std::bool_constant<!std::is_abstract_v<ObjectT> && std::is_default_constructible_v<ObjectT>>{});
    }

    template<class Object, class Archive>
    template<class Data>
    void TableScribe<Object, Archive>::TableBase<Data>::PushToObjectImplementation(
        DataT& data, ObjectT& object)
    {}

    template<class Object, class Archive>
    template<class Data>
    void TableScribe<Object, Archive>::TableBase<Data>::PullFromObjectImplementation(
        DataT& data, ObjectT& object)
    {}

    template<class Object, class Archive>
    template<class Data>
    void TableScribe<Object, Archive>::TableBase<Data>::AddDataEntry(DataEntry&& entry)
    {
        dataEntryList.push_back(std::move(entry));
    }

    template<class Object, class Archive>
    template<class Data>
    void TableScribe<Object, Archive>::TableBase<Data>::MergeDataEntries(DataEntryList&& entries)
    {
        dataEntryList.insert(
            dataEntryList.end(),
            std::make_move_iterator(entries.begin()),
            std::make_move_iterator(entries.end()));
        entries.clear();
    }

    template<class Object, class Archive>
    template<class Data>
    void TableScribe<Object, Archive>::TableBase<Data>::DoConstruct(
        DataT& data, ObjectT*& object, ArchiveT& archive, std::true_type)
    {
        object = new ObjectT();
    }

    template<class Object, class Archive>
    template<class Data>
    void TableScribe<Object, Archive>::TableBase<Data>::DoConstruct(
        DataT& data, ObjectT*& object, ArchiveT& archive, std::false_type)
    {}
}

#include "UndefAssert.h"