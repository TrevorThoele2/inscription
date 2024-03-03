#pragma once

#include "TableData.h"

#include "ScrivenDispatch.h"

#include "ObjectTrackingContext.h"

namespace Inscription
{
    class ConstructDispatch
    {
    private:
        template<class T>
        using Table = typename Scribe<T>::Table;

        template<class T>
        using KnownType = remove_const_t<T>;
        template<class T>
        using KnownScribe = Scribe<KnownType<T>>;
        template<class T, class Archive>
        using KnownScribeTraits = ScribeTraits<KnownType<T>, Archive>;
        template<class T, class Archive>
        using KnownScribeCategory = typename KnownScribeTraits<T, Archive>::Category;
    public:
        template<class T, class... Args>
        static auto is_braces_constructible_impl(int) -> decltype(void(T{ std::declval<Args>()... }), std::true_type())
        {
            return {};
        }

        template<class T, class... Args>
        static auto is_braces_constructible_impl(...) -> std::false_type
        {
            return {};
        }

        template<class T, class... Args>
        struct is_braces_constructible : decltype(is_braces_constructible_impl<T, Args...>(0))
        {};

        template<class T, class... Args>
        static constexpr bool is_braces_constructible_v = is_braces_constructible<T, Args...>::value;

        template<class T>
        struct is_braces_default_constructible : is_braces_constructible<T>
        {};

        template<class T>
        static constexpr bool is_braces_default_constructible_v = is_braces_constructible<T>::value;
    public:
        template<class T, class Archive, class = void>
        struct scribe_has_custom_construct : std::false_type
        {};

        template<class T, class Archive>
        struct scribe_has_custom_construct<
            T,
            Archive,
            std::void_t<decltype(
                std::declval<Scribe<T>>().Construct(std::declval<T*>(), std::declval<Archive&>()))>
        >
            : std::true_type
        {};

        template<class T, class Archive>
        static constexpr bool scribe_has_custom_construct_v = scribe_has_custom_construct<T, Archive>::value;
    public:
        template<class T, class Archive, class = void>
        struct category_has_custom_construct : std::false_type
        {};

        template<class T, class Archive>
        struct category_has_custom_construct<
            T,
            Archive,
            std::void_t<decltype(
                KnownScribeCategory<T, Archive>::Construct(std::declval<T*>(), std::declval<Archive&>()))>
        >
            : std::true_type
        {};

        template<class T, class Archive>
        static constexpr bool category_has_custom_construct_v = category_has_custom_construct<T, Archive>::value;
    private:
        template<class T, class Archive, class = void>
        struct table_has_custom_construct : std::false_type
        {};

        template<class T, class Archive>
        struct table_has_custom_construct<
            T,
            Archive,
            std::void_t<decltype(
                std::declval<Table<T>>().Construct(std::declval<T*>(), std::declval<Archive&>()))>
        >
            : std::true_type
        {};

        template<class T, class Archive>
        static constexpr bool table_has_custom_construct_v = table_has_custom_construct<T, Archive>::value;
    public:
        template<
            class T,
            class Archive,
            std::enable_if_t<scribe_has_custom_construct<T, Archive>::value, int> = 0
        > static void Execute(T* storage, Archive& archive)
        {
            archive.types.AttemptTrackObject(storage);

            auto trackingContext = ObjectTrackingContext::Active(archive.types);

            Scribe<T> scribe;
            scribe.Construct(storage, archive);
        }

        template<
            class T,
            class Archive,
            std::enable_if_t<
                std::conjunction<
                    std::negation<scribe_has_custom_construct<T, Archive>>,
                    category_has_custom_construct<T, Archive>>::value,
            int> = 0
        > static void Execute(T* storage, Archive& archive)
        {
            archive.types.AttemptTrackObject(storage);

            auto trackingContext = ObjectTrackingContext::Active(archive.types);

            KnownScribeCategory<T, Archive>::Construct(storage, archive);
        }

        template<
            class T,
            class Archive,
            std::enable_if_t<
                std::conjunction<
                    std::negation<scribe_has_custom_construct<T, Archive>>,
                    std::negation<category_has_custom_construct<T, Archive>>,
                    std::negation<std::is_abstract<T>>,
                    is_braces_default_constructible<T>>::value,
            int> = 0
        > static void Execute(T* storage, Archive& archive)
        {
            archive.types.AttemptTrackObject(storage);

            auto trackingContext = ObjectTrackingContext::Active(archive.types);
            new (storage) T{};
            ScrivenDispatch::Execute(*storage, archive);
        }

        template<
            class T,
            class Archive,
            std::enable_if_t<
                std::conjunction<
                    std::negation<scribe_has_custom_construct<T, Archive>>,
                    std::negation<category_has_custom_construct<T, Archive>>,
                    std::negation<std::is_abstract<T>>,
                    std::negation<is_braces_default_constructible<T>>>::value,
            int> = 0
        > static void Execute(T* storage, Archive& archive)
        {
            static_assert(false,
                "Basic construction of this object requires a default constructor, "
                "Construct method in the Scribe, "
                "or Construct method in the Scribe's Category. ");
        }

        template<
            class T,
            class Archive,
            std::enable_if_t<
                std::conjunction<
                    std::negation<scribe_has_custom_construct<T, Archive>>,
                    std::is_abstract<T>>::value,
            int> = 0
        > static void Execute(T* storage, Archive& archive)
        {}

        template<
            class T,
            class Archive,
            std::enable_if_t<
                std::conjunction<
                    std::negation<scribe_has_custom_construct<T, Archive>>,
                    std::negation<std::is_abstract<T>>,
                    is_braces_default_constructible<T>>::value,
            int> = 0
        > static void NamedExecute(
            const std::string& name, T* storage, Archive& archive)
        {
            archive.types.AttemptTrackObject(storage);

            auto trackingContext = ObjectTrackingContext::Active(archive.types);
            new (storage) T{};
            ScrivenDispatch::NamedExecute(name, *storage, archive);
        }
    public:
        template<
            class T,
            class Archive,
            std::enable_if_t<table_has_custom_construct<T, Archive>::value, int> = 0
        > static void TableExecute(T* storage, Archive& archive, Table<T>& table)
        {
            table.Construct(storage, archive);
        }

        template<
            class T,
            class Archive,
            std::enable_if_t<
                std::conjunction<
                    std::negation<table_has_custom_construct<T, Archive>>,
                    std::negation<std::is_abstract<T>>,
                    is_braces_default_constructible<T>>::value,
            int> = 0
        > static void TableExecute(T* storage, Archive& archive, Table<T>& table)
        {
            new (storage) T{};
        }

        template<
            class T,
            class Archive,
            std::enable_if_t<
                std::conjunction<
                    std::negation<table_has_custom_construct<T, Archive>>,
                    std::negation<std::is_abstract<T>>,
                    std::negation<is_braces_default_constructible<T>>,
                    is_braces_constructible<T, const TableData<T, Archive>&>>::value,
            int> = 0
        > static void TableExecute(T* storage, Archive& archive, Table<T>& table)
        {
            new (storage) T{ table.data };
        }

        template<
            class T,
            class Archive,
            std::enable_if_t<
                std::conjunction<
                    std::negation<table_has_custom_construct<T, Archive>>,
                    std::negation<std::is_abstract<T>>,
                    std::negation<is_braces_default_constructible<T>>,
                    std::negation<is_braces_constructible<T, const TableData<T, Archive>&>>>::value,
            int> = 0
        > static void TableExecute(T* storage, Archive& archive, Table<T>& table)
        {
            static_assert(false,
                "Basic table construction requires either a "
                "default constructor or a constructor taking a (const DataT&) on ObjectT.");
        }

        template<
            class T,
            class Archive,
            std::enable_if_t<
                std::is_abstract<T>::value,
            int> = 0
        > static void TableExecute(T* storage, Archive& archive, Table<T>& table)
        {}
    };
}