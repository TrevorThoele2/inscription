#pragma once

#include <type_traits>
#include "Scribe.h"

namespace Inscription
{
    template<class T, class Format, class = void>
    struct scribe_has_input_types
        : std::false_type
    {};

    template<class T, class Format>
    struct scribe_has_input_types<
        T,
        Format,
        std::void_t<decltype(Scribe<T>::InputTypes(std::declval<const Format&>()))>
    >
        : std::true_type
    {};

    template<class T, class Format>
    static constexpr bool scribe_has_input_types_v =
        scribe_has_input_types<T, Format>::value;
}