#pragma once

#include <type_traits>
#include <vector>

#include "TableData.h"
#include "TableDataBase.h"
#include "AutoTableDataLink.h"

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
        DataT data = {};
    public:
        template<class T>
        using ComposingScribe = Scribe<T, ArchiveT>;
    public:
        void Scriven(ArchiveT& archive);
        void ObjectScriven(ObjectT& object, ArchiveT& archive);

        void PushToObject(ObjectT& object, ArchiveT& archive);
        void PullFromObject(ObjectT& object, ArchiveT& archive);
    public:
        virtual ~TableBase() = 0;
    protected:
        TableBase() = default;
    protected:
        virtual void ScrivenImplementation(ArchiveT& archive);
        virtual void ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive);

        virtual void PushToObjectImplementation(ObjectT& object, ArchiveT& archive);
        virtual void PullFromObjectImplementation(ObjectT& object, ArchiveT& archive);
    protected:
        using DataLink = AutoTableDataLink<TableBase, ObjectT, ArchiveT>;
        using DataLinkList = std::vector<DataLink>;
        void AddDataLink(DataLink&& link);
        void MergeDataLinks(DataLinkList&& links);
    private:
        DataLinkList dataEntryList;
    private:
        template<class T, class U>
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
    TableBase<Object, Archive>::~TableBase() = default;

    template<class Object, class Archive>
    void TableBase<Object, Archive>::ScrivenImplementation(
        ArchiveT& archive)
    {}

    template<class Object, class Archive>
    void TableBase<Object, Archive>::ObjectScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
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
}