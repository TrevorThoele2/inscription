
#pragma once

namespace Inscription
{
    enum TrackEnum
    {
        TRACK_NEVER = 0,
        TRACK_SELECTIVELY = 1,
        TRACK_ALWAYS = 2
    };

    template<class T>
    struct TrackPointer
    { static constexpr TrackEnum value = TRACK_SELECTIVELY; };

    template<>
    struct TrackPointer<bool>
    { static constexpr TrackEnum value = TRACK_NEVER; };

    template<>
    struct TrackPointer<char>
    { static constexpr TrackEnum value = TRACK_NEVER; };

    template<>
    struct TrackPointer<short>
    { static constexpr TrackEnum value = TRACK_NEVER; };

    template<>
    struct TrackPointer<int>
    { static constexpr TrackEnum value = TRACK_NEVER; };

    template<>
    struct TrackPointer<long long>
    { static constexpr TrackEnum value = TRACK_NEVER; };

    template<>
    struct TrackPointer<unsigned char>
    { static constexpr TrackEnum value = TRACK_NEVER; };

    template<>
    struct TrackPointer<unsigned short>
    { static constexpr TrackEnum value = TRACK_NEVER; };

    template<>
    struct TrackPointer<unsigned int>
    { static constexpr TrackEnum value = TRACK_NEVER; };

    template<>
    struct TrackPointer<unsigned long long>
    { static constexpr TrackEnum value = TRACK_NEVER; };

    template<>
    struct TrackPointer<float>
    { static constexpr TrackEnum value = TRACK_NEVER; };

    template<>
    struct TrackPointer<double>
    { static constexpr TrackEnum value = TRACK_NEVER; };
}