#pragma once

#include <typeindex>
#include "TrackingID.h"

namespace Inscription
{
    class TypeTrackerEntry
    {
    public:
        using ID = TrackingID;
        using Type = std::type_index;
    public:
        ID id;
        Type type;
        bool hasSavedConstruction = false;
    public:
        TypeTrackerEntry();
        TypeTrackerEntry(ID id, const Type& type);
    };
}