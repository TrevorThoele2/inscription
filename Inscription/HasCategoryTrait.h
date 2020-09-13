#pragma once

#include <type_traits>

namespace Inscription
{
    template<class T, class Enable = void>
    struct has_category_trait : std::false_type
    {};

    template<class T>
    struct has_category_trait<T, std::void_t<typename T::Category>> : std::true_type
    {};

    template<class T>
    static constexpr bool has_category_trait_v = has_category_trait<T>::value;
}