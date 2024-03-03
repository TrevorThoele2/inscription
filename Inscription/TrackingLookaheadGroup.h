#pragma once

#include <vector>

#include "TrackingLookaheadEntry.h"

namespace Inscription
{
    class TrackingLookaheadGroup
    {
    public:
        typedef std::vector<TrackingLookaheadEntry> List;
        List list;
    public:
        TrackingLookaheadGroup() = default;
        TrackingLookaheadGroup(const TrackingLookaheadGroup& arg) = default;
        TrackingLookaheadGroup(TrackingLookaheadGroup&& arg);

        TrackingLookaheadGroup& operator=(const TrackingLookaheadGroup& arg) = default;
        TrackingLookaheadGroup& operator=(TrackingLookaheadGroup&& arg);
    };
}