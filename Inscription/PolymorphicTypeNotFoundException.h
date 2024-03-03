#pragma once

#include "Exception.h"

namespace Inscription
{
    class PolymorphicTypeNotFoundException : public Exception
    {
    public:
        PolymorphicTypeNotFoundException();
    };
}