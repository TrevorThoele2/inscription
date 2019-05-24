#pragma once

#include "Exception.h"

namespace Inscription
{
    class TableNotLinked : public Exception
    {
    public:
        TableNotLinked();
    };
}