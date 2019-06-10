#pragma once

#include <limits>
#include "TrackingID.h"

namespace Inscription
{
    struct SpecialTypeTrackingID
    {
        static const TrackingID NULLPTR = std::numeric_limits<TrackingID>::max() - 1;
        static const TrackingID HANDLE_NONPOLYMORPHICALLY = std::numeric_limits<TrackingID>::max();
    };
}