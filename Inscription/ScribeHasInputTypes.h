#pragma once

#include <type_traits>
#include "Scribe.h"

namespace Inscription
{
    template<class T, class Archive, class = void>
    struct scribe_has_input_types
        : std::false_type
    {};

    template<class T, class Archive>
    struct scribe_has_input_types<
        T,
        Archive,
        std::void_t<decltype(Scribe<T>::InputTypes(std::declval<const Archive&>()))>
    >
        : std::true_type
    {};

    template<class T, class Archive>
    static constexpr bool scribe_has_input_types_v =
        scribe_has_input_types<T, Archive>::value;
}