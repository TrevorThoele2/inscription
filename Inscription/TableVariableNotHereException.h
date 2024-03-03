#pragma once

#include "Exception.h"

namespace Inscription
{
    class TableVariableNotHereException : public Exception
    {
    public:
        TableVariableNotHereException();
    };
}