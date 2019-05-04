#pragma once

#include <type_traits>

#include "PolymorphicID.h"

namespace Inscription
{
    class PointerDelegate
    {
    public:
        PointerDelegate();
        virtual ~PointerDelegate() = 0;
    protected:
        PolymorphicID nextID;
    };
}