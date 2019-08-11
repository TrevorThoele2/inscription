#pragma once

#include "TrackingID.h"

namespace Inscription
{
    class ObjectTrackerEntry
    {
    public:
        using ID = TrackingID;
        const ID id = 0;
        bool hasSavedConstruction = false;
        void* object = nullptr;
    public:
        ObjectTrackerEntry() = default;
        ObjectTrackerEntry(ID id, void* object);
        ObjectTrackerEntry(const ObjectTrackerEntry& arg);
    };
}