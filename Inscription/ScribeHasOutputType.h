#pragma once

#include <type_traits>
#include "Scribe.h"

namespace Inscription
{
    template<class T, class Archive, class = void>
    struct scribe_has_output_type
        : std::false_type
    {};

    template<class T, class Archive>
    struct scribe_has_output_type<
        T,
        Archive,
        std::void_t<decltype(Scribe<T>::OutputType(std::declval<const Archive&>()))>
    >
        : std::true_type
    {};

    template<class T, class Archive>
    static constexpr bool scribe_has_output_type_v =
        scribe_has_output_type<T, Archive>::value;
}