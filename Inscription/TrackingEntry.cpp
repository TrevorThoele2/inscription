#include "TrackingEntry.h"

namespace Inscription
{
    TrackingEntry::TrackingEntry() : id(invalidTrackingID), obj(nullptr), hasBeenSerialized(false)
    {}

    TrackingEntry::TrackingEntry(ID id, void *obj) : id(id), obj(obj), hasBeenSerialized(false)
    {}

    TrackingEntry::TrackingEntry(const TrackingEntry &arg) : id(arg.id), obj(arg.obj), hasBeenSerialized(arg.hasBeenSerialized)
    {}
}