#pragma once

#include "Exception.h"

namespace Inscription
{
    class TableNotConstructedException : public Exception
    {
    public:
        TableNotConstructedException();
    };
}