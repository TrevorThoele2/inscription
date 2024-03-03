#pragma once

#include <limits>
#include "TrackingID.h"

namespace Inscription
{
    struct SpecialObjectTrackingID
    {
        static const TrackingID NULLPTR = std::numeric_limits<TrackingID>::max();
    };
}