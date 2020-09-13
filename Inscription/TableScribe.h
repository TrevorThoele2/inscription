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
        using BaseScribeT::Scriven;
        void Scriven(ObjectT& object, ArchiveT& archive) override final;
        void Construct(ObjectT* storage, ArchiveT& archive);
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override final;
    private:
        template<class T, class A, class = void>
        struct scribe_has_table : std::false_type
        {};

        template<class T, class A>
        struct scribe_has_table<T, A,
            std::void_t<typename Scribe<T, A>::Table>> : std::true_type
        {};

#define REQUIRE_TABLE \
static_assert(scribe_has_table<ObjectT, ArchiveT>::value, "TableScribe's require a Table object. Declare one.")

        template<class T, class A, std::enable_if_t<scribe_has_table<T, A>::value, int> = 0>
        void DoScriven(T& object, A& archive)
        {
            {
                auto trackingID = archive.types.AttemptTrackObject(&object);
                if (trackingID.has_value())
                    archive.types.TrackSavedConstruction(*trackingID);
            }

            {
                auto trackingContext = ObjectTrackingContext::Active(archive.types);
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

        template<class T, class A, std::enable_if_t<!scribe_has_table<T, A>::value, int> = 0>
        void DoScriven(T& object, A& archive)
        {
            REQUIRE_TABLE;
        }

        template<class T, class A, std::enable_if_t<scribe_has_table<T, A>::value, int> = 0>
        void DoConstruct(T* storage, A& archive)
        {
            {
                archive.types.AttemptTrackObject(storage);
            }

            {
                auto trackingContext = ObjectTrackingContext::Active(archive.types);
                using TableT = typename Scribe<ObjectT, ArchiveT>::Table;

                TableT table;
                table.Scriven(archive);
                Access::TableConstruct(storage, archive, table);
                table.PushToObject(*storage, archive);
                table.ObjectScriven(*storage, archive);
            }
        }

        template<class T, class A, std::enable_if_t<!scribe_has_table<T, A>::value, int> = 0>
        void DoConstruct(T* storage, A& archive)
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