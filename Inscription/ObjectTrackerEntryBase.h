#pragma once

#include "TrackingID.h"

namespace Inscription
{
    class ObjectTrackerEntryBase
    {
    public:
        using ID = TrackingID;
        const ID id = 0;
    public:
        virtual ~ObjectTrackerEntryBase() = 0;

        virtual ObjectTrackerEntryBase* Clone() const = 0;
    protected:
        ObjectTrackerEntryBase() = default;
        ObjectTrackerEntryBase(ID id);
        ObjectTrackerEntryBase(const ObjectTrackerEntryBase& arg) = default;
    };
}