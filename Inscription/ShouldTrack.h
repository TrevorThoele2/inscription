#pragma once

#include <type_traits>
#include "ScribeTraits.h"

namespace Inscription
{
    template<class T, class Format, class Enable = void>
    struct has_should_track_trait : std::false_type
    {};

    template<class T, class Format>
    struct has_should_track_trait<
        T,
        Format,
        std::void_t<decltype(ScribeTraits<T, Format>::shouldTrack)>> : std::true_type
    {};

    template<class T, class Format>
    static constexpr bool has_should_track_trait_v = has_should_track_trait<T, Format>::value;

    template<class T, class Format, class = void>
    struct should_track : std::true_type
    {};

    template<class T, class Format>
    struct should_track<
        T,
        Format,
        std::void_t<decltype(has_should_track_trait_v<T, Format> && !ScribeTraits<T, Format>::shouldTrack)>
    >
        : std::true_type
    {};

    template<class T, class Format>
    static constexpr bool should_track_v = should_track<T, Format>::value;
}