#pragma once

#include <limits>
#include <string>
#include "TrackingID.h"

namespace Inscription
{
    struct SpecialObjectTrackingID
    {
        static const TrackingID NULLPTR = std::numeric_limits<TrackingID>::max();
    };

    struct SpecialObjectTrackingJsonID
    {
        static inline const std::string NULLPTR = "null";
    };
}