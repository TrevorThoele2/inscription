#include "TypeTrackerEntry.h"

#include "SpecialTypeTrackingID.h"

namespace Inscription
{
    TypeTrackerEntry::TypeTrackerEntry() : id(SpecialTypeTrackingID::NULLPTR), type(typeid(nullptr_t))
    {}

    TypeTrackerEntry::TypeTrackerEntry(ID id, const Type& type) : id(id), type(type)
    {}
}