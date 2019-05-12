#pragma once

#include "Exception.h"

namespace Inscription
{
    class InvalidScribeDirection : public Exception
    {
    public:
        InvalidScribeDirection();
    };
}