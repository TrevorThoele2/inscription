#pragma once

#include "Exception.h"

namespace Inscription
{
    class PolymorphicTypeNotFound : public Exception
    {
    public:
        PolymorphicTypeNotFound();
    };
}