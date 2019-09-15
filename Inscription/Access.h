#pragma once

#include "ObjectTrackingContext.h"

namespace Inscription
{
    class Access
    {
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
            static_assert(!std::is_abstract_v<Object> && !is_braces_default_constructible_v<Object>,
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
    };
}

#define INSCRIPTION_ACCESS              \
template<class Object, class Archive>   \
friend class ::Inscription::Scribe;     \
friend ::Inscription::Access

#define INSCRIPTION_TABLE_ACCESS        \
template<class Object, class Archive>   \
friend class ::Inscription::Scribe;     \
friend ::Inscription::Access