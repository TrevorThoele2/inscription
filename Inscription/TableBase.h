#pragma once

#include <type_traits>
#include <vector>

#include "AutoTableDataEntry.h"

#include "Scribe.h"

namespace Inscription
{
    template<class Object, class Archive, class Data>
    class TableBase
    {
    public:
        using ObjectT = Object;
        using ArchiveT = Archive;
    public:
        using DataT = Data;
        DataT data;
    public:
        template<class T>
        using ComposingScribe = Scribe<T, ArchiveT>;
    public:
        void Scriven(ArchiveT& archive);
        void ObjectScriven(ObjectT& object, ArchiveT& archive);
        ObjectT* Construct(ArchiveT& archive);

        void PushToObject(ObjectT& object);
        void PullFromObject(ObjectT& object);
    protected:
        virtual void ScrivenImplementation(ArchiveT& archive);
        virtual void ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive);

        virtual void PushToObjectImplementation(ObjectT& object);
        virtual void PullFromObjectImplementation(ObjectT& object);
    protected:
        using DataEntry = AutoTableDataEntry<TableBase, ObjectT, ArchiveT>;
        using DataEntryList = std::vector<DataEntry>;
        void AddDataEntry(DataEntry&& entry);
        void MergeDataEntries(DataEntryList&& entries);
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
        ObjectT* DoConstruct(ArchiveT& archive)
        {
            return new ObjectT();
        }

        template<class T, std::enable_if_t<shouldDataConstruct<T>, int> = 0>
        ObjectT* DoConstruct(ArchiveT& archive)
        {
            using DerivedData = typename Scribe<ObjectT, ArchiveT>::Data;
            return DataConstructionImplementation<Data, DerivedData>::template Do<T>(data);
        }

        template<class T, std::enable_if_t<shouldIgnoreConstruct<T>, int> = 0>
        ObjectT* DoConstruct(ArchiveT& archive)
        {
            static_assert(false,
                "Non-shadowed construction requires either a default constructor or a constructor taking a TableT. "
                "Create one of these constructors or shadow Construct.");

            return nullptr;
        }

        template<class BaseData, class DerivedData>
        struct DataConstructionImplementation
        {
            template<class T, std::enable_if_t<std::is_constructible_v<T, const DerivedData&>, int> = 0>
            static ObjectT* Do(BaseData& data)
            {
                auto& derivedData = static_cast<const DerivedData&>(data);
                return new ObjectT(derivedData);
            }

            template<class T, std::enable_if_t<!std::is_constructible_v<T, const DerivedData&>, int> = 0>
            static ObjectT* Do(BaseData& table)
            {
                static_assert(
                    false,
                    "Non-shadowed construction requires either a"
                    "default constructor or a constructor taking a (const DataT&) on ObjectT. "

                    "Create one of these constructors or shadow Construct.");
            }
        };
    private:
        template<class Object, class Archive>
        friend class TableScribe;
    };

    template<class Object, class Archive, class Data>
    void TableBase<Object, Archive, Data>::Scriven(ArchiveT& archive)
    {
        for (auto& loop : dataEntryList)
            loop.Scriven(*this, archive);

        ScrivenImplementation(archive);
    }

    template<class Object, class Archive, class Data>
    void TableBase<Object, Archive, Data>::ObjectScriven(ObjectT& object, ArchiveT& archive)
    {
        for (auto& loop : dataEntryList)
            loop.ObjectScriven(*this, object, archive);

        ObjectScrivenImplementation(object, archive);
    }

    template<class Object, class Archive, class Data>
    auto TableBase<Object, Archive, Data>::Construct(ArchiveT& archive) -> ObjectT*
    {
        return DoConstruct<ObjectT>(archive);
    }

    template<class Object, class Archive, class Data>
    void TableBase<Object, Archive, Data>::PushToObject(ObjectT& object)
    {
        for (auto& loop : dataEntryList)
            loop.PushToObject(*this, object);

        PushToObjectImplementation(object);
    }

    template<class Object, class Archive, class Data>
    void TableBase<Object, Archive, Data>::PullFromObject(ObjectT& object)
    {
        for (auto& loop : dataEntryList)
            loop.PullFromObject(*this, object);

        PullFromObjectImplementation(object);
    }

    template<class Object, class Archive, class Data>
    void TableBase<Object, Archive, Data>::ScrivenImplementation(
        ArchiveT& archive)
    {}

    template<class Object, class Archive, class Data>
    void TableBase<Object, Archive, Data>::ObjectScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {}

    template<class Object, class Archive, class Data>
    void TableBase<Object, Archive, Data>::PushToObjectImplementation(
        ObjectT& object)
    {}

    template<class Object, class Archive, class Data>
    void TableBase<Object, Archive, Data>::PullFromObjectImplementation(
        ObjectT& object)
    {}

    template<class Object, class Archive, class Data>
    void TableBase<Object, Archive, Data>::AddDataEntry(DataEntry&& entry)
    {
        dataEntryList.push_back(std::move(entry));
    }

    template<class Object, class Archive, class Data>
    void TableBase<Object, Archive, Data>::MergeDataEntries(DataEntryList&& entries)
    {
        dataEntryList.insert(
            dataEntryList.end(),
            std::make_move_iterator(entries.begin()),
            std::make_move_iterator(entries.end()));
        entries.clear();
    }
}