#pragma once

#include "ObjectTrackerEntryBase.h"

namespace Inscription
{
    class ObjectTrackerEntry : public ObjectTrackerEntryBase
    {
    public:
        void* object = nullptr;
    public:
        ObjectTrackerEntry() = default;
        ObjectTrackerEntry(ID id, void* object);
        ObjectTrackerEntry(const ObjectTrackerEntry& arg);

        ObjectTrackerEntry* Clone() const override;
    };
}