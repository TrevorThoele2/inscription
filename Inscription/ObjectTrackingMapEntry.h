#pragma once

#include "TrackingID.h"

namespace Inscription
{
    class ObjectTrackingMapEntry
    {
    public:
        typedef TrackingID ID;
    public:
        const ID id;
        void* obj;
        bool hasBeenSerialized;
    public:
        ObjectTrackingMapEntry();
        ObjectTrackingMapEntry(ID id, void* obj);
        ObjectTrackingMapEntry(const ObjectTrackingMapEntry& arg);
    };
}