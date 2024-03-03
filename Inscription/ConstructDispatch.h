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
    public:
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
        template<class T, class Format>
        static void Execute(T* storage, Format& format)
        {
            static_assert(
                scribe_has_custom_construct_v<T, Format>
                || category_has_custom_construct_v<T, Format>
                || is_braces_constructible_v<T>
                || std::is_abstract_v<T>,

                "Basic construction of this object requires a default constructor, "
                "Construct method in the Scribe, "
                "or Construct method in the Scribe's Category. ");

            if constexpr (!std::is_abstract_v<T>)
            {
                format.types.AttemptTrackObject(storage);
                auto trackingContext = ObjectTrackingContext::Active(format.types);

                if constexpr (scribe_has_custom_construct_v<T, Format>)
                {
                    Scribe<T> scribe;
                    scribe.Construct(storage, format);
                }
                else if constexpr (category_has_custom_construct_v<T, Format>)
                {
                    KnownScribeCategory<T, Format>::Construct(storage, format);
                }
                else if constexpr (is_braces_default_constructible_v<T>)
                {
                    new (storage) T{};
                    ScrivenDispatch::Execute(*storage, format);
                }
            }
        }
        template<class T, class Format>
        static void NamedExecute(const std::string& name, T* storage, Format& format)
        {
            static_assert(
                !scribe_has_custom_construct_v<T, Format>
                || is_braces_default_constructible_v<T>
                || std::is_abstract_v<T>);

            format.types.AttemptTrackObject(storage);

            auto trackingContext = ObjectTrackingContext::Active(format.types);
            new (storage) T{};
            ScrivenDispatch::NamedExecute(name, *storage, format);
        }
    public:
        template<class T, class Format>
        static void TableExecute(T* storage, Format& format, Table<T>& table)
        {
            static_assert(
                table_has_custom_construct_v<T, Format>
                || is_braces_default_constructible_v<T>
                || is_braces_constructible_v<T, const TableData<T, Format>&>
                || std::is_abstract_v<T>,

                "Basic table construction requires either a "
                "default constructor or a constructor taking a (const DataT&) on ObjectT.");

            if constexpr (!std::is_abstract_v<T>)
            {
                if constexpr (table_has_custom_construct_v<T, Format>)
                    table.Construct(storage, format);
                else if constexpr (is_braces_constructible_v<T>)
                    new (storage) T{};
                else if constexpr ( is_braces_constructible_v<T, const TableData<T, Format>&>)
                    new (storage) T{ table.data };
            }
        }
    };
}