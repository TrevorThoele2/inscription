#pragma once

#include "CompositeScribe.h"

#include "TableBase.h"

namespace Inscription
{
    class BinaryArchive;

    template<class Object, class Archive>
    class TableScribe;

    template<class Object>
    class TableScribe<Object, BinaryArchive> : public CompositeScribe<Object, BinaryArchive>
    {
    private:
        using BaseScribeT = CompositeScribe<Object, BinaryArchive>;
    public:
        using ObjectT = typename BaseScribeT::ObjectT;
        using ArchiveT = typename BaseScribeT::ArchiveT;

        using TableBase = TableBase<ObjectT, ArchiveT>;
    public:
        void Scriven(ObjectT& object, ArchiveT& archive) override final;
        void Construct(ObjectT* storage, ArchiveT& archive);
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override final;
    private:
        template<class Object, class Archive, class = void>
        struct scribe_has_table : std::false_type
        {};

        template<class Object, class Archive>
        struct scribe_has_table<
            Object,
            Archive,
            std::void_t<typename Scribe<Object, Archive>::Table>> : std::true_type
        {};

#define REQUIRE_TABLE \
static_assert(scribe_has_table<ObjectT, ArchiveT>::value, "TableScribe's require a Table object. Declare one.")

        template<class Object, class Archive, std::enable_if_t<scribe_has_table<Object, Archive>::value, int> = 0>
        void DoScriven(Object& object, Archive& archive)
        {
            {
                auto trackingID = archive.AttemptTrackObject(&object);
                if (trackingID.IsValid())
                    archive.TrackSavedConstruction(*trackingID);
            }

            {
                ObjectTrackingContext trackingContext(ObjectTrackingContext::Active, archive);
                using TableT = typename Scribe<ObjectT, ArchiveT>::Table;

                TableT table;
                if (archive.IsOutput())
                {
                    table.PullFromObject(object, archive);
                    table.Scriven(archive);
                    table.ObjectScriven(object, archive);
                }
                else
                {
                    table.Scriven(archive);
                    table.PushToObject(object, archive);
                    table.ObjectScriven(object, archive);
                }
            }
        }

        template<class Object, class Archive, std::enable_if_t<!scribe_has_table<Object, Archive>::value, int> = 0>
        void DoScriven(Object& object, Archive& archive)
        {
            REQUIRE_TABLE;
        }

        template<class Object, class Archive, std::enable_if_t<scribe_has_table<Object, Archive>::value, int> = 0>
        void DoConstruct(Object* storage, Archive& archive)
        {
            {
                archive.AttemptTrackObject(storage);
            }

            {
                ObjectTrackingContext trackingContext(ObjectTrackingContext::Active, archive);
                using TableT = typename Scribe<ObjectT, ArchiveT>::Table;

                TableT table;
                table.Scriven(archive);
                Access::TableConstruct(storage, archive, table);
                table.PushToObject(*storage, archive);
                table.ObjectScriven(*storage, archive);
            }
        }

        template<class Object, class Archive, std::enable_if_t<!scribe_has_table<Object, Archive>::value, int> = 0>
        void DoConstruct(Object* storage, Archive& archive)
        {
            REQUIRE_TABLE;
        }

#undef REQUIRE_TABLE

    private:
        static_assert(std::is_class_v<ObjectT>,
            "The Object given to a TableScribe was not a composite.");
    };

    template<class Object>
    void TableScribe<Object, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        DoScriven(object, archive);
    }

    template<class Object>
    void TableScribe<Object, BinaryArchive>::Construct(ObjectT* storage, ArchiveT& archive)
    {
        DoConstruct(storage, archive);
    }

    template<class Object>
    void TableScribe<Object, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {}
}