#pragma once

#include "Exception.h"

namespace Inscription
{
    class RegisteredTypeNotFoundException : public Exception
    {
    public:
        RegisteredTypeNotFoundException();
    };
}