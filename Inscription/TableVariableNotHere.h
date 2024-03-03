#pragma once

#include "Exception.h"

namespace Inscription
{
    class TableVariableNotHere : public Exception
    {
    public:
        TableVariableNotHere();
    };
}