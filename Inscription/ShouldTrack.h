#pragma once

#include <type_traits>
#include "ScribeTraits.h"

namespace Inscription
{
    template<class T, class Archive, class Enable = void>
    struct has_should_track_trait : std::false_type
    {};

    template<class T, class Archive>
    struct has_should_track_trait<
        T,
        Archive,
        std::void_t<decltype(ScribeTraits<T, Archive>::shouldTrack)>> : std::true_type
    {};

    template<class T, class Archive>
    static constexpr bool has_should_track_trait_v = has_should_track_trait<T, Archive>::value;

    template<class T, class Archive, class = void>
    struct should_track : std::true_type
    {};

    template<class T, class Archive>
    struct should_track<
        T,
        Archive,
        std::void_t<decltype(has_should_track_trait_v<T, Archive> && !ScribeTraits<T, Archive>::shouldTrack)>
    >
        : std::true_type
    {};

    template<class T, class Archive>
    static constexpr bool should_track_v = should_track<T, Archive>::value;
}