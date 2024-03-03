#pragma once

#include "TableData.h"

#include "ObjectTrackingContext.h"

namespace Inscription
{
    class Access
    {
    private:
        template<class Object, class Archive>
        using Table = typename Scribe<Object, Archive>::Table;
    private:
        template<class T, class... Args>
        static auto is_constructible_impl(int) -> decltype(void(T{ std::declval<Args>()... }), std::true_type())
        {
            return {};
        }

        template<class T, class... Args>
        static auto is_constructible_impl(...) -> std::false_type
        {
            return {};
        }

        template<class T, class... Args>
        struct is_constructible : decltype(is_constructible_impl<T, Args...>(0))
        {};

        template<class T, class... Args>
        static constexpr bool is_braces_constructible_v = is_constructible<T, Args...>::value;

        template<class T, class... Args>
        struct is_braces_default_constructible : decltype(is_constructible<T>(0))
        {};

        template<class T, class... Args>
        static constexpr bool is_braces_default_constructible_v = is_constructible<T>::value;
    private:
        template<class Object, class Archive, class = void>
        struct scribe_has_custom_construct : std::false_type
        {};

        template<class Object, class Archive>
        struct scribe_has_custom_construct<
            Object,
            Archive,
            std::void_t<decltype(
                std::declval<Scribe<Object, Archive>>().Construct(std::declval<Object*>(), std::declval<Archive&>()))>
        >
            : std::true_type
        {};

        template<class Object, class Archive>
        static constexpr bool scribe_has_custom_construct_v = scribe_has_custom_construct<Object, Archive>::value;
    private:
        template<class Object, class Archive, class = void>
        struct table_has_custom_construct : std::false_type
        {};

        template<class Object, class Archive>
        struct table_has_custom_construct<
            Object,
            Archive,
            std::void_t<decltype(
                std::declval<Table<Object, Archive>>().Construct(std::declval<Object*>(), std::declval<Archive&>()))>
        >
            : std::true_type
        {};

        template<class Object, class Archive>
        static constexpr bool table_has_custom_construct_v = table_has_custom_construct<Object, Archive>::value;
    public:
        template<
            class Object,
            class Archive,
            std::enable_if_t<scribe_has_custom_construct_v<Object, Archive>, int> = 0
        > static void Construct(Object* storage, Archive& archive, Scribe<Object, Archive>& scribe)
        {
            {
                archive.AttemptTrackObject(storage);
            }

            ObjectTrackingContext trackingContext(ObjectTrackingContext::Active, archive);
            scribe.Construct(storage, archive);
        }

        template<
            class Object,
            class Archive,
            std::enable_if_t<
                !std::is_abstract_v<Object> && is_braces_default_constructible_v<Object>
                && !scribe_has_custom_construct_v<Object, Archive>,
            int> = 0
        > static void Construct(Object* storage, Archive& archive, Scribe<Object, Archive>& scribe)
        {
            {
                archive.AttemptTrackObject(storage);
            }

            ObjectTrackingContext trackingContext(ObjectTrackingContext::Active, archive);
            new (storage) Object{};
            scribe.Scriven(*storage, archive);
        }

        template<
            class Object,
            class Archive,
            std::enable_if_t<
                !std::is_abstract_v<Object> && !is_braces_default_constructible_v<Object>
                && !scribe_has_custom_construct_v<Object, Archive>,
            int> = 0
        > static void Construct(Object* storage, Archive& archive, Scribe<Object, Archive>& scribe)
        {
            static_assert(false,
                "Basic construction of this object requires a default constructor. "
                "Define it or override ConstructImplementation for this Scribe.");
        }

        template<
            class Object,
            class Archive,
            std::enable_if_t<
                std::is_abstract_v<Object>
                && !scribe_has_custom_construct_v<Object, Archive>,
            int> = 0
        > static void Construct(Object* storage, Archive& archive, Scribe<Object, Archive>& scribe)
        {}
    public:
        template<
            class Object,
            class Archive,
            std::enable_if_t<table_has_custom_construct_v<Object, Archive>, int> = 0
        > static void TableConstruct(Object* storage, Archive& archive, Table<Object, Archive>& table)
        {
            table.Construct(storage, archive);
        }

        template<
            class Object,
            class Archive,
            std::enable_if_t<
                !std::is_abstract_v<Object> &&
                !table_has_custom_construct_v<Object, Archive> &&
                is_braces_default_constructible_v<Object>,
            int> = 0
        > static void TableConstruct(Object* storage, Archive& archive, Table<Object, Archive>& table)
        {
            new (storage) Object{};
        }

        template<
            class Object,
            class Archive,
            std::enable_if_t<
                !std::is_abstract_v<Object> &&
                !table_has_custom_construct_v<Object, Archive> &&
                !is_braces_default_constructible_v<Object> &&
                is_braces_constructible_v<Object, const TableData<Object, Archive>&>,
            int> = 0
        > static void TableConstruct(Object* storage, Archive& archive, Table<Object, Archive>& table)
        {
            new (storage) Object{ table.data };
        }

        template<
            class Object,
            class Archive,
            std::enable_if_t<
                !std::is_abstract_v<Object> &&
                !table_has_custom_construct_v<Object, Archive> &&
                !is_braces_default_constructible_v<Object> &&
                !is_braces_constructible_v<Object, const TableData<Object, Archive>&>,
            int> = 0
        > static void TableConstruct(Object* storage, Archive& archive, Table<Object, Archive>& table)
        {
            static_assert(
                !is_braces_constructible_v<Object, const TableData<Object, Archive>&>,
                "Basic table construction requires either a "
                "default constructor or a constructor taking a (const DataT&) on ObjectT. "

                "Create one of these constructors or manually construct in ConstructImplementation.");
        }

        template<
            class Object,
            class Archive,
            std::enable_if_t<
                std::is_abstract_v<Object>,
            int> = 0
        > static void TableConstruct(Object* storage, Archive& archive, Table<Object, Archive>& table)
        {}
    };
}

#define INSCRIPTION_ACCESS              \
friend ::Inscription::Access;           \
template<class Object, class Archive>   \
friend class ::Inscription::Scribe

#define INSCRIPTION_TABLE_ACCESS        \
friend ::Inscription::Access;           \
template<class Object, class Archive>   \
friend class ::Inscription::Scribe;     \
template<class Object, class Archive>   \
friend struct ::Inscription::TableData