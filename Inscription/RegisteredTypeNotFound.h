#pragma once

#include "Exception.h"

namespace Inscription
{
    class RegisteredTypeNotFound : public Exception
    {
    public:
        RegisteredTypeNotFound();
    };
}