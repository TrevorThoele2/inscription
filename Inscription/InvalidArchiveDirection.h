#pragma once

#include "Exception.h"

namespace Inscription
{
    class InvalidArchiveDirection : public Exception
    {
    public:
        InvalidArchiveDirection();
    };
}