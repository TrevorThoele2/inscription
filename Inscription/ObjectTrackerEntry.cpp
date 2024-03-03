#include "ObjectTrackerEntry.h"

namespace Inscription
{
    ObjectTrackerEntry::ObjectTrackerEntry(ID id, void* object) :
        id(id), object(object)
    {}

    ObjectTrackerEntry::ObjectTrackerEntry(const ObjectTrackerEntry& arg) :
        id(arg.id), object(arg.object)
    {}
}