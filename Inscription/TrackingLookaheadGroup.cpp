#include "TrackingLookaheadGroup.h"

namespace Inscription
{
    TrackingLookaheadGroup::TrackingLookaheadGroup(TrackingLookaheadGroup&& arg) : list(std::move(arg.list))
    {}

    TrackingLookaheadGroup& TrackingLookaheadGroup::operator=(TrackingLookaheadGroup&& arg)
    {
        list = std::move(arg.list);
        return *this;
    }
}