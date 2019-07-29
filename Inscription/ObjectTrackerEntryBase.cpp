#include "ObjectTrackerEntryBase.h"

namespace Inscription
{
    ObjectTrackerEntryBase::~ObjectTrackerEntryBase()
    {}

    ObjectTrackerEntryBase::ObjectTrackerEntryBase(ID id) :
        id(id)
    {}

    ObjectTrackerEntryBase& ObjectTrackerEntryBase::operator=(const ObjectTrackerEntryBase& arg)
    {
        hasSavedConstruction = arg.hasSavedConstruction;
        return *this;
    }
}