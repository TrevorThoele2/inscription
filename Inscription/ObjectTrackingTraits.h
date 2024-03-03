#pragma once

namespace Inscription
{
    template<class Object, class Archive>
    struct ObjectTrackingTraits
    {
        static constexpr bool shouldTrack = true;
    };
}