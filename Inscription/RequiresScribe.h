#pragma once

#include <type_traits>

namespace Inscription
{
    template<class T, class Enable = void>
    struct has_requires_scribe_trait : std::false_type
    {};

    template<class T>
    struct has_requires_scribe_trait<T, std::void_t<decltype(T::requiresScribe)>> : std::true_type
    {};

    template<class T>
    static constexpr bool has_requires_scribe_trait_v = has_requires_scribe_trait<T>::value;

    template <class T, class Enable = void>
    struct requires_scribe : std::false_type
    {};

    template <class T>
    struct requires_scribe<T, std::enable_if_t<!has_requires_scribe_trait_v<T>>> : std::true_type
    {};

    template <class T>
    struct requires_scribe<T, std::enable_if_t<T::requiresScribe>> : std::true_type
    {};

    template<class T>
    static constexpr bool requires_scribe_v = requires_scribe<T>::value;
}