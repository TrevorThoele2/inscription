#pragma once

#include "PolymorphicID.h"

namespace Inscription
{
    enum PointerSpecialIDs : PolymorphicID
    {
        NULLPTR = 0,
        HANDLE_NONPOLYMORPHICALLY = 1,
        ID_START = 2
    };
}