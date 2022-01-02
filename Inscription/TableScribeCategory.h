#pragma once

#include "TableBase.h"

namespace Inscription
{
    namespace Format
    {
        class Binary;
    }

    template<class Object>
    class TableScribeCategory final
    {
    public:
        using ObjectT = Object;
    public:
        static constexpr bool requiresScribe = false;
    public:
        static void Scriven(ObjectT& object, Format::Binary& format);
        static void Construct(ObjectT* storage, Format::Binary& format);
    private:
        template<class T, class Format, class = void>
        struct scribe_has_table : std::false_type
        {};

        template<class T, class Format>
        struct scribe_has_table<T, Format,
            std::void_t<typename Scribe<T>::Table>> : std::true_type
        {};

#define REQUIRE_TABLE(FormatT) \
static_assert(scribe_has_table<ObjectT, FormatT>::value, "TableScribe's require a declared Table object.")

        template<class T, std::enable_if_t<scribe_has_table<T, Format::Binary>::value, int> = 0>
        static void DoScriven(T& object, Format::Binary& format)
        {
            {
                auto trackingID = format.types.AttemptTrackObject(&object);
                if (trackingID.has_value())
                    format.types.TrackSavedConstruction(*trackingID);
            }

            {
                auto trackingContext = ObjectTrackingContext::Active(format.types);
                using TableT = typename Scribe<ObjectT>::Table;

                TableT table;
                if (format.IsOutput())
                {
                    table.PullFromObject(object, format);
                    table.Scriven(format);
                    table.ObjectScriven(object, format);
                }
                else
                {
                    table.Scriven(format);
                    table.PushToObject(object, format);
                    table.ObjectScriven(object, format);
                }
            }
        }

        template<class T, std::enable_if_t<!scribe_has_table<T, Format::Binary>::value, int> = 0>
        static void DoScriven(T& object, Format::Binary& format)
        {
            REQUIRE_TABLE(Format::Binary);
        }

        template<class T, std::enable_if_t<scribe_has_table<T, Format::Binary>::value, int> = 0>
        static void DoConstruct(T* storage, Format::Binary& format)
        {
            {
                format.types.AttemptTrackObject(storage);
            }

            {
                auto trackingContext = ObjectTrackingContext::Active(format.types);
                using TableT = typename Scribe<ObjectT>::Table;

                TableT table;
                table.Scriven(format);
                ConstructDispatch::TableExecute(storage, format, table);
                table.PushToObject(*storage, format);
                table.ObjectScriven(*storage, format);
            }
        }

        template<class T, std::enable_if_t<!scribe_has_table<T, Format::Format>::value, int> = 0>
        static void DoConstruct(T* storage, Format::Binary& format)
        {
            REQUIRE_TABLE(Format::Binary);
        }

#undef REQUIRE_TABLE

    private:
        static_assert(std::is_class_v<ObjectT>,
            "The Object given to a TableScribeCategory is not a class/struct.");
    };

    template<class Object>
    void TableScribeCategory<Object>::Scriven(ObjectT& object, Format::Binary& format)
    {
        DoScriven(object, format);
    }

    template<class Object>
    void TableScribeCategory<Object>::Construct(ObjectT* storage, Format::Binary& format)
    {
        DoConstruct(storage, format);
    }
}