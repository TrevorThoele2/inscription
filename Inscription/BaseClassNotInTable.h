#pragma once

#include "Exception.h"
#include <typeindex>

namespace Inscription
{
    class BaseClassNotInTable : public Exception
    {
    public:
        BaseClassNotInTable(const std::type_index& type);
    };
}