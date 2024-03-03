#pragma once

#include "TableBase.h"

namespace Inscription
{
    class BinaryArchive;

    template<class Object>
    class TableScribeCategory final
    {
    public:
        using ObjectT = Object;
    public:
        static constexpr bool requiresScribe = false;
    public:
        static void Scriven(ObjectT& object, BinaryArchive& archive);
        static void Construct(ObjectT* storage, BinaryArchive& archive);
    private:
        template<class T, class Archive, class = void>
        struct scribe_has_table : std::false_type
        {};

        template<class T, class Archive>
        struct scribe_has_table<T, Archive,
            std::void_t<typename Scribe<T>::Table>> : std::true_type
        {};

#define REQUIRE_TABLE(ArchiveT) \
static_assert(scribe_has_table<ObjectT, ArchiveT>::value, "TableScribe's require a declared Table object.")

        template<class T, std::enable_if_t<scribe_has_table<T, BinaryArchive>::value, int> = 0>
        static void DoScriven(T& object, BinaryArchive& archive)
        {
            {
                auto trackingID = archive.types.AttemptTrackObject(&object);
                if (trackingID.has_value())
                    archive.types.TrackSavedConstruction(*trackingID);
            }

            {
                auto trackingContext = ObjectTrackingContext::Active(archive.types);
                using TableT = typename Scribe<ObjectT>::Table;

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

        template<class T, std::enable_if_t<!scribe_has_table<T, BinaryArchive>::value, int> = 0>
        static void DoScriven(T& object, BinaryArchive& archive)
        {
            REQUIRE_TABLE(BinaryArchive);
        }

        template<class T, std::enable_if_t<scribe_has_table<T, BinaryArchive>::value, int> = 0>
        static void DoConstruct(T* storage, BinaryArchive& archive)
        {
            {
                archive.types.AttemptTrackObject(storage);
            }

            {
                auto trackingContext = ObjectTrackingContext::Active(archive.types);
                using TableT = typename Scribe<ObjectT>::Table;

                TableT table;
                table.Scriven(archive);
                ConstructDispatch::TableExecute(storage, archive, table);
                table.PushToObject(*storage, archive);
                table.ObjectScriven(*storage, archive);
            }
        }

        template<class T, std::enable_if_t<!scribe_has_table<T, Archive>::value, int> = 0>
        static void DoConstruct(T* storage, BinaryArchive& archive)
        {
            REQUIRE_TABLE(BinaryArchive);
        }

#undef REQUIRE_TABLE

    private:
        static_assert(std::is_class_v<ObjectT>,
            "The Object given to a TableScribeCategory is not a class/struct.");
    };

    template<class Object>
    void TableScribeCategory<Object>::Scriven(ObjectT& object, BinaryArchive& archive)
    {
        DoScriven(object, archive);
    }

    template<class Object>
    void TableScribeCategory<Object>::Construct(ObjectT* storage, BinaryArchive& archive)
    {
        DoConstruct(storage, archive);
    }
}