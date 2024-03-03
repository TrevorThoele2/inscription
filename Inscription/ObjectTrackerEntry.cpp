#include "ObjectTrackerEntry.h"

namespace Inscription
{
    ObjectTrackerEntry::ObjectTrackerEntry(ID id, void* object) :
        ObjectTrackerEntryBase(id), object(object)
    {}

    ObjectTrackerEntry::ObjectTrackerEntry(const ObjectTrackerEntry& arg) :
        ObjectTrackerEntryBase(arg), object(arg.object)
    {}

    ObjectTrackerEntry* ObjectTrackerEntry::Clone() const
    {
        return new ObjectTrackerEntry(*this);
    }
}