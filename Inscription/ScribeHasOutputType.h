#pragma once

#include <type_traits>
#include "Scribe.h"

namespace Inscription
{
    template<class T, class Format, class = void>
    struct scribe_has_output_type
        : std::false_type
    {};

    template<class T, class Format>
    struct scribe_has_output_type<
        T,
        Format,
        std::void_t<decltype(Scribe<T>::OutputType(std::declval<const Format&>()))>
    >
        : std::true_type
    {};

    template<class T, class Format>
    static constexpr bool scribe_has_output_type_v =
        scribe_has_output_type<T, Format>::value;
}