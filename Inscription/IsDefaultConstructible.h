#pragma once

#include <type_traits>

namespace Inscription
{
    template<typename T, typename = void>
    struct IsDefaultConstructible : std::false_type
    {};

    template<typename T>
    struct IsDefaultConstructible<T, typename std::enable_if<true, decltype(T(), (void)0)>::type> : std::true_type
    {};
}