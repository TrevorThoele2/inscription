#include "RegisteredTypes.h"

#include "TrackerMap.h"

namespace Inscription
{
    void RegisteredTypes::CopyRegisteredTo(TrackerMap& group)
    {
        for (auto& loop : Instance().bases)
            group.table.Add(Tracker(), loop->Type());
    }

    void RegisteredTypes::CopyRegisteredTo(PointerManager& manager, BinaryScribe& scribe)
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