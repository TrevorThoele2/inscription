#pragma once

#include <typeindex>
#include "Exception.h"

namespace Inscription
{
    class RegisteredTypeNotFound : public Exception
    {
    public:
        RegisteredTypeNotFound(const std::type_index& type);
    };
}