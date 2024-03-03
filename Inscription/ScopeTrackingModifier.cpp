#include "ScopeTrackingModifier.h"

#include "BinaryArchive.h"

namespace Inscription
{
    ScopeTrackingModifier::ScopeTrackingModifier(BinaryArchive& archive, bool shouldTrack) :
        archive(&archive), previousValue(archive.TrackObjects(shouldTrack))
    {}

    ScopeTrackingModifier::~ScopeTrackingModifier()
    {
        archive->TrackObjects(previousValue);
    }
}