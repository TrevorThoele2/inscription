#pragma once

#include <type_traits>
#include <vector>

#include "TableData.h"
#include "TableDataBase.h"
#include "AutoTableDataEntry.h"

#include "Scribe.h"

namespace Inscription
{
    template<class Object, class Archive>
    class TableBase
    {
    public:
        using ObjectT = Object;
        using ArchiveT = Archive;
    public:
        using DataT = TableData<ObjectT, ArchiveT>;
        DataT data;
    public:
        template<class T>
        using ComposingScribe = Scribe<T, ArchiveT>;
    public:
        void Scriven(ArchiveT& archive);
        void ObjectScriven(ObjectT& object, ArchiveT& archive);
        void Construct(ObjectT* storage, ArchiveT& archive);

        void PushToObject(ObjectT& object, ArchiveT& archive);
        void PullFromObject(ObjectT& object, ArchiveT& archive);
    protected:
        virtual void ScrivenImplementation(ArchiveT& archive);
        virtual void ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive);
        virtual void ConstructImplementation(ObjectT* storage, ArchiveT& archive) = 0;

        virtual void PushToObjectImplementation(ObjectT& object, ArchiveT& archive);
        virtual void PullFromObjectImplementation(ObjectT& object, ArchiveT& archive);
    protected:
        using DataEntry = AutoTableDataEntry<TableBase, ObjectT, ArchiveT>;
        using DataEntryList = std::vector<DataEntry>;
        void AddDataEntry(DataEntry&& entry);
        void MergeDataEntries(DataEntryList&& entries);
    protected:
        void DoBasicConstruction(ObjectT* storage, ArchiveT& archive);
    private:
        DataEntryList dataEntryList;
    private:
        template<class T>
        constexpr static bool shouldDefaultConstruct = !std::is_abstract_v<T> && std::is_default_constructible_v<T>;
        template<class T>
        constexpr static bool shouldDataConstruct = !std::is_abstract_v<T> && !std::is_default_constructible_v<T>;
        template<class T>
        constexpr static bool shouldIgnoreConstruct = std::is_abstract_v<T>;

        template<class T, std::enable_if_t<shouldDefaultConstruct<T>, int> = 0>
        void BasicConstructionImplementation(ObjectT* storage, ArchiveT& archive)
        {
            new (storage) ObjectT();
        }

        template<class T, std::enable_if_t<shouldDataConstruct<T>, int> = 0>
        void BasicConstructionImplementation(ObjectT* storage, ArchiveT& archive)
        {
            DataConstructionImplementation<T>(storage, data);
        }

        template<class T, std::enable_if_t<shouldIgnoreConstruct<T>, int> = 0>
        void BasicConstructionImplementation(ObjectT* storage, ArchiveT& archive)
        {}

        template<class T, std::enable_if_t<std::is_constructible_v<T, const DataT&>, int> = 0>
        static void DataConstructionImplementation(ObjectT* storage, DataT& data)
        {
            new (storage) ObjectT(data);
        }

        template<class T, std::enable_if_t<!std::is_constructible_v<T, const DataT&>, int> = 0>
        static void DataConstructionImplementation(ObjectT* storage, DataT& table)
        {
            static_assert(
                false,
                "Basic table construction requires either a "
                "default constructor or a constructor taking a (const DataT&) on ObjectT. "

                "Create one of these constructors or manually construct in ConstructImplementation.");
        }
    private:
        template<class Object, class Archive>
        friend class TableScribe;
    };

    template<class Object, class Archive>
    void TableBase<Object, Archive>::Scriven(ArchiveT& archive)
    {
        for (auto& loop : dataEntryList)
            loop.Scriven(*this, archive);

        ScrivenImplementation(archive);
    }

    template<class Object, class Archive>
    void TableBase<Object, Archive>::ObjectScriven(ObjectT& object, ArchiveT& archive)
    {
        for (auto& loop : dataEntryList)
            loop.ObjectScriven(*this, object, archive);

        ObjectScrivenImplementation(object, archive);
    }

    template<class Object, class Archive>
    void TableBase<Object, Archive>::Construct(ObjectT* storage, ArchiveT& archive)
    {
        ConstructImplementation(storage, archive);
    }

    template<class Object, class Archive>
    void TableBase<Object, Archive>::PushToObject(ObjectT& object, ArchiveT& archive)
    {
        for (auto& loop : dataEntryList)
            loop.PushToObject(*this, object, archive);

        PushToObjectImplementation(object, archive);
    }

    template<class Object, class Archive>
    void TableBase<Object, Archive>::PullFromObject(ObjectT& object, ArchiveT& archive)
    {
        for (auto& loop : dataEntryList)
            loop.PullFromObject(*this, object, archive);

        PullFromObjectImplementation(object, archive);
    }

    template<class Object, class Archive>
    void TableBase<Object, Archive>::ScrivenImplementation(
        ArchiveT& archive)
    {}

    template<class Object, class Archive>
    void TableBase<Object, Archive>::ObjectScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {}

    template<class Object, class Archive>
    void TableBase<Object, Archive>::ConstructImplementation(ObjectT* storage, ArchiveT& archive)
    {}

    template<class Object, class Archive>
    void TableBase<Object, Archive>::PushToObjectImplementation(
        ObjectT& object, ArchiveT& archive)
    {}

    template<class Object, class Archive>
    void TableBase<Object, Archive>::PullFromObjectImplementation(
        ObjectT& object, ArchiveT& archive)
    {}

    template<class Object, class Archive>
    void TableBase<Object, Archive>::AddDataEntry(DataEntry&& entry)
    {
        dataEntryList.push_back(std::move(entry));
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

    template<class Object, class Archive>
    void TableBase<Object, Archive>::DoBasicConstruction(ObjectT* storage, ArchiveT& archive)
    {
        BasicConstructionImplementation<ObjectT>(storage, archive);
    }
}