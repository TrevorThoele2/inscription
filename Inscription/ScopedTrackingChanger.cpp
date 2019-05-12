#include "ScopedTrackingChanger.h"

#include "BinaryScribe.h"

namespace Inscription
{
    ScopedTrackingChanger::ScopedTrackingChanger(BinaryScribe& scribe, bool shouldTrack) :
        scribe(&scribe), previousValue(scribe.TrackObjects(shouldTrack))
    {}

    ScopedTrackingChanger::~ScopedTrackingChanger()
    {
        scribe->TrackObjects(previousValue);
    }
}