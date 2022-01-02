#pragma once

#include <type_traits>
#include <vector>

#include "TableData.h"
#include "TableDataBase.h"
#include "AutoTableDataLink.h"

#include "Scribe.h"

namespace Inscription
{
    template<class Object, class Format>
    class TableBase
    {
    public:
        using ObjectT = Object;
        using FormatT = Format;
    public:
        using DataT = TableData<ObjectT, FormatT>;
        DataT data = {};
    public:
        template<class T>
        using ComposingScribe = Scribe<T>;
    public:
        void Scriven(FormatT& format);
        void ObjectScriven(ObjectT& object, FormatT& format);

        void PushToObject(ObjectT& object, FormatT& format);
        void PullFromObject(ObjectT& object, FormatT& format);
    public:
        virtual ~TableBase() = 0;
    protected:
        TableBase() = default;
    protected:
        virtual void ScrivenImplementation(FormatT& format);
        virtual void ObjectScrivenImplementation(ObjectT& object, FormatT& format);

        virtual void PushToObjectImplementation(ObjectT& object, FormatT& format);
        virtual void PullFromObjectImplementation(ObjectT& object, FormatT& format);
    protected:
        using DataLink = AutoTableDataLink<TableBase, ObjectT, FormatT>;
        using DataLinkList = std::vector<DataLink>;
        void AddDataLink(DataLink&& link);
        void MergeDataLinks(DataLinkList&& links);
    private:
        DataLinkList dataEntryList{};
    private:
        template<class T, class U>
        friend class TableScribe;
    };

    template<class Object, class Format>
    void TableBase<Object, Format>::Scriven(FormatT& format)
    {
        for (auto& loop : dataEntryList)
            loop.Scriven(*this, format);

        ScrivenImplementation(format);
    }

    template<class Object, class Format>
    void TableBase<Object, Format>::ObjectScriven(ObjectT& object, FormatT& format)
    {
        for (auto& loop : dataEntryList)
            loop.ObjectScriven(*this, object, format);

        format.types.AttemptTrackObject(&object);

        ObjectScrivenImplementation(object, format);
    }

    template<class Object, class Format>
    void TableBase<Object, Format>::PushToObject(ObjectT& object, FormatT& format)
    {
        for (auto& loop : dataEntryList)
            loop.PushToObject(*this, object, format);

        PushToObjectImplementation(object, format);
    }

    template<class Object, class Format>
    void TableBase<Object, Format>::PullFromObject(ObjectT& object, FormatT& format)
    {
        for (auto& loop : dataEntryList)
            loop.PullFromObject(*this, object, format);

        PullFromObjectImplementation(object, format);
    }

    template <class Object, class Format>
    TableBase<Object, Format>::~TableBase() = default;

    template<class Object, class Format>
    void TableBase<Object, Format>::ScrivenImplementation(
        FormatT& format)
    {}

    template<class Object, class Format>
    void TableBase<Object, Format>::ObjectScrivenImplementation(
        ObjectT& object, FormatT& format)
    {}

    template<class Object, class Format>
    void TableBase<Object, Format>::PushToObjectImplementation(
        ObjectT& object, FormatT& format)
    {}

    template<class Object, class Format>
    void TableBase<Object, Format>::PullFromObjectImplementation(
        ObjectT& object, FormatT& format)
    {}

    template<class Object, class Format>
    void TableBase<Object, Format>::AddDataLink(DataLink&& link)
    {
        dataEntryList.push_back(std::move(link));
    }

    template<class Object, class Format>
    void TableBase<Object, Format>::MergeDataLinks(DataLinkList&& links)
    {
        dataEntryList.insert(
            dataEntryList.end(),
            std::make_move_iterator(links.begin()),
            std::make_move_iterator(links.end()));
        links.clear();
    }
}