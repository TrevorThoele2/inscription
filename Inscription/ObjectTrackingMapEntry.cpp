#include "ObjectTrackingMapEntry.h"

namespace Inscription
{
    ObjectTrackingMapEntry::ObjectTrackingMapEntry() : id(0), obj(nullptr), hasBeenSerialized(false)
    {}

    ObjectTrackingMapEntry::ObjectTrackingMapEntry(ID id, void* obj) : id(id), obj(obj), hasBeenSerialized(false)
    {}

    ObjectTrackingMapEntry::ObjectTrackingMapEntry(const ObjectTrackingMapEntry& arg) : id(arg.id), obj(arg.obj), hasBeenSerialized(arg.hasBeenSerialized)
    {}
}