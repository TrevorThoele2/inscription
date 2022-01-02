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
        template<class T, class Format>
        using KnownScribeTraits = ScribeTraits<KnownType<T>, Format>;
        template<class T, class Format>
        using KnownScribeCategory = typename KnownScribeTraits<T, Format>::Category;
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
        template<class T, class Format, class = void>
        struct scribe_has_custom_construct : std::false_type
        {};

        template<class T, class Format>
        struct scribe_has_custom_construct<
            T,
            Format,
            std::void_t<decltype(
                std::declval<Scribe<T>>().Construct(std::declval<T*>(), std::declval<Format&>()))>
        >
            : std::true_type
        {};

        template<class T, class Format>
        static constexpr bool scribe_has_custom_construct_v = scribe_has_custom_construct<T, Format>::value;
    public:
        template<class T, class Format, class = void>
        struct category_has_custom_construct : std::false_type
        {};

        template<class T, class Format>
        struct category_has_custom_construct<
            T,
            Format,
            std::void_t<decltype(
                KnownScribeCategory<T, Format>::Construct(std::declval<T*>(), std::declval<Format&>()))>
        >
            : std::true_type
        {};

        template<class T, class Format>
        static constexpr bool category_has_custom_construct_v = category_has_custom_construct<T, Format>::value;
    private:
        template<class T, class Format, class = void>
        struct table_has_custom_construct : std::false_type
        {};

        template<class T, class Format>
        struct table_has_custom_construct<
            T,
            Format,
            std::void_t<decltype(
                std::declval<Table<T>>().Construct(std::declval<T*>(), std::declval<Format&>()))>
        >
            : std::true_type
        {};

        template<class T, class Format>
        static constexpr bool table_has_custom_construct_v = table_has_custom_construct<T, Format>::value;
    public:
        template<
            class T,
            class Format,
            std::enable_if_t<scribe_has_custom_construct<T, Format>::value, int> = 0
        > static void Execute(T* storage, Format& format)
        {
            format.types.AttemptTrackObject(storage);

            auto trackingContext = ObjectTrackingContext::Active(format.types);

            Scribe<T> scribe;
            scribe.Construct(storage, format);
        }

        template<
            class T,
            class Format,
            std::enable_if_t<
                std::conjunction<
                    std::negation<scribe_has_custom_construct<T, Format>>,
                    category_has_custom_construct<T, Format>>::value,
            int> = 0
        > static void Execute(T* storage, Format& format)
        {
            format.types.AttemptTrackObject(storage);

            auto trackingContext = ObjectTrackingContext::Active(format.types);

            KnownScribeCategory<T, Format>::Construct(storage, format);
        }

        template<
            class T,
            class Format,
            std::enable_if_t<
                std::conjunction<
                    std::negation<scribe_has_custom_construct<T, Format>>,
                    std::negation<category_has_custom_construct<T, Format>>,
                    std::negation<std::is_abstract<T>>,
                    is_braces_default_constructible<T>>::value,
            int> = 0
        > static void Execute(T* storage, Format& format)
        {
            format.types.AttemptTrackObject(storage);

            auto trackingContext = ObjectTrackingContext::Active(format.types);
            new (storage) T{};
            ScrivenDispatch::Execute(*storage, format);
        }

        template<
            class T,
            class Format,
            std::enable_if_t<
                std::conjunction<
                    std::negation<scribe_has_custom_construct<T, Format>>,
                    std::negation<category_has_custom_construct<T, Format>>,
                    std::negation<std::is_abstract<T>>,
                    std::negation<is_braces_default_constructible<T>>>::value,
            int> = 0
        > static void Execute(T* storage, Format& format)
        {
            static_assert(false,
                "Basic construction of this object requires a default constructor, "
                "Construct method in the Scribe, "
                "or Construct method in the Scribe's Category. ");
        }

        template<
            class T,
            class Format,
            std::enable_if_t<
                std::conjunction<
                    std::negation<scribe_has_custom_construct<T, Format>>,
                    std::is_abstract<T>>::value,
            int> = 0
        > static void Execute(T* storage, Format& format)
        {}

        template<
            class T,
            class Format,
            std::enable_if_t<
                std::conjunction<
                    std::negation<scribe_has_custom_construct<T, Format>>,
                    std::negation<std::is_abstract<T>>,
                    is_braces_default_constructible<T>>::value,
            int> = 0
        > static void NamedExecute(
            const std::string& name, T* storage, Format& format)
        {
            format.types.AttemptTrackObject(storage);

            auto trackingContext = ObjectTrackingContext::Active(format.types);
            new (storage) T{};
            ScrivenDispatch::NamedExecute(name, *storage, format);
        }
    public:
        template<
            class T,
            class Format,
            std::enable_if_t<table_has_custom_construct<T, Format>::value, int> = 0
        > static void TableExecute(T* storage, Format& format, Table<T>& table)
        {
            table.Construct(storage, format);
        }

        template<
            class T,
            class Format,
            std::enable_if_t<
                std::conjunction<
                    std::negation<table_has_custom_construct<T, Format>>,
                    std::negation<std::is_abstract<T>>,
                    is_braces_default_constructible<T>>::value,
            int> = 0
        > static void TableExecute(T* storage, Format& format, Table<T>& table)
        {
            new (storage) T{};
        }

        template<
            class T,
            class Format,
            std::enable_if_t<
                std::conjunction<
                    std::negation<table_has_custom_construct<T, Format>>,
                    std::negation<std::is_abstract<T>>,
                    std::negation<is_braces_default_constructible<T>>,
                    is_braces_constructible<T, const TableData<T, Format>&>>::value,
            int> = 0
        > static void TableExecute(T* storage, Format& format, Table<T>& table)
        {
            new (storage) T{ table.data };
        }

        template<
            class T,
            class Format,
            std::enable_if_t<
                std::conjunction<
                    std::negation<table_has_custom_construct<T, Format>>,
                    std::negation<std::is_abstract<T>>,
                    std::negation<is_braces_default_constructible<T>>,
                    std::negation<is_braces_constructible<T, const TableData<T, Format>&>>>::value,
            int> = 0
        > static void TableExecute(T* storage, Format& format, Table<T>& table)
        {
            static_assert(false,
                "Basic table construction requires either a "
                "default constructor or a constructor taking a (const DataT&) on ObjectT.");
        }

        template<
            class T,
            class Format,
            std::enable_if_t<
                std::is_abstract<T>::value,
            int> = 0
        > static void TableExecute(T* storage, Format& format, Table<T>& table)
        {}
    };
}