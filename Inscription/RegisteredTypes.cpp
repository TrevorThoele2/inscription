
#include "RegisteredTypes.h"

#include "Tracking.h"

namespace Inscription
{
    void RegisteredTypes::PushToTracking(TrackerGroup& group)
    {
        for (auto& loop : Instance().bases)
            group.table.Add(::Inscription::Tracker(), loop->Type());
    }

    void RegisteredTypes::PushToPolymorphic(PointerManager& manager, Scribe& scribe)
    {
        for (auto& loop : Instance().bases)
            loop->FillPolymorphic(manager, scribe);
    }

    RegisteredTypes& RegisteredTypes::Instance()
    {
        static RegisteredTypes instance;
        return instance;
    }

    RegisteredTypes::Base::~Base() 
    {}
}