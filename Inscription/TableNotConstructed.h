#pragma once

#include "Exception.h"

namespace Inscription
{
    class TableNotConstructed : public Exception
    {
    public:
        TableNotConstructed();
    };
}