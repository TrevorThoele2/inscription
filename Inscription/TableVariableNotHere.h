#pragma once

#include "Exception.h"
#include <string>

namespace Inscription
{
    class TableVariableNotHere : public Exception
    {
    public:
        TableVariableNotHere(const std::string& name);
    };
}