#pragma once

#include "Exception.h"

namespace Inscription
{
    class InvalidSignature : public Exception
    {
    public:
        InvalidSignature();
    };
}