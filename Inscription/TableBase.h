#pragma once

#include <type_traits>
#include <vector>

#include "TableData.h"
#include "TableDataBase.h"
#include "AutoTableDataLink.h"

#include "Scribe.h"

#include <Chroma/IsBracesConstructible.h>

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
    public:
        virtual ~TableBase() = 0;
    protected:
        TableBase() {};
    protected:
        virtual void ScrivenImplementation(ArchiveT& archive);
        virtual void ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive);
        virtual void ConstructImplementation(ObjectT* storage, ArchiveT& archive) = 0;

        virtual void PushToObjectImplementation(ObjectT& object, ArchiveT& archive);
        virtual void PullFromObjectImplementation(ObjectT& object, ArchiveT& archive);
    protected:
        using DataLink = AutoTableDataLink<TableBase, ObjectT, ArchiveT>;
        using DataLinkList = std::vector<DataLink>;
        void AddDataLink(DataLink&& link);
        void MergeDataLinks(DataLinkList&& links);
    protected:
        void DoBasicConstruction(ObjectT* storage, ArchiveT& archive);
    private:
        DataLinkList dataEntryList;
    private:
        template<class T>
        constexpr static bool shouldDefaultConstruct =
            !std::is_abstract_v<T> &&
            ::Chroma::is_braces_default_constructible_v<T>;
        template<class T>
        constexpr static bool shouldDataConstruct =
            !std::is_abstract_v<T> &&
            !::Chroma::is_braces_default_constructible_v<T>;
        template<class T>
        constexpr static bool shouldIgnoreConstruct =
            std::is_abstract_v<T>;

        template<class T, std::enable_if_t<shouldDefaultConstruct<T>, int> = 0>
        void BasicConstructionImplementation(ObjectT* storage, ArchiveT& archive)
        {
            new (storage) ObjectT{};
        }

        template<class T, std::enable_if_t<shouldDataConstruct<T>, int> = 0>
        void BasicConstructionImplementation(ObjectT* storage, ArchiveT& archive)
        {
            DataConstructionImplementation<T>(storage, data);
        }

        template<class T, std::enable_if_t<shouldIgnoreConstruct<T>, int> = 0>
        void BasicConstructionImplementation(ObjectT* storage, ArchiveT& archive)
        {}

        template<class T, std::enable_if_t<Chroma::is_braces_constructible_v<T, const DataT&>, int> = 0>
        static void DataConstructionImplementation(ObjectT* storage, DataT& data)
        {
            new (storage) ObjectT{ data };
        }

        template<class T, std::enable_if_t<!Chroma::is_braces_constructible_v<T, const DataT&>, int> = 0>
        static void DataConstructionImplementation(ObjectT* storage, DataT& table)
        {
            static_assert(
                !Chroma::is_braces_constructible_v<T, const DataT&>,
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

        archive.AttemptTrackObject(&object);

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

    template <class Object, class Archive>
    TableBase<Object, Archive>::~TableBase()
    {}

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
    void TableBase<Object, Archive>::AddDataLink(DataLink&& link)
    {
        dataEntryList.push_back(std::move(link));
    }

    template<class Object, class Archive>
    void TableBase<Object, Archive>::MergeDataLinks(DataLinkList&& links)
    {
        dataEntryList.insert(
            dataEntryList.end(),
            std::make_move_iterator(links.begin()),
            std::make_move_iterator(links.end()));
        links.clear();
    }

    template<class Object, class Archive>
    void TableBase<Object, Archive>::DoBasicConstruction(ObjectT* storage, ArchiveT& archive)
    {
        BasicConstructionImplementation<ObjectT>(storage, archive);
    }
}