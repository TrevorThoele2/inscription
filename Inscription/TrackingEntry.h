#pragma once

#include "TrackingID.h"

namespace Inscription
{
    class TrackingEntry
    {
    public:
        typedef TrackingID ID;
    public:
        const ID id;
        void* obj;
        bool hasBeenSerialized;
    public:
        TrackingEntry();
        TrackingEntry(ID id, void* obj);
        TrackingEntry(const TrackingEntry& arg);
    };
}