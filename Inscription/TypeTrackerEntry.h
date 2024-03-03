#pragma once

#include <typeindex>
#include "TrackingID.h"

namespace Inscription
{
    class TypeTrackerEntry
    {
    public:
        typedef TrackingID ID;
        typedef std::type_index Type;
    public:
        ID id;
        Type type;
    public:
        TypeTrackerEntry();
        TypeTrackerEntry(ID id, const Type& type);
    };
}