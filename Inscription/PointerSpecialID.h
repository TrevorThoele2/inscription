#pragma once

#include "PolymorphicID.h"

namespace Inscription
{
    enum PointerSpecialID : PolymorphicID
    {
        NULLPTR = 0,
        HANDLE_NONPOLYMORPHICALLY = 1,
        ID_START = 2
    };
}