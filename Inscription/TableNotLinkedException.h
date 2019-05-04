#pragma once

#include "Exception.h"

namespace Inscription
{
    class TableNotLinkedException : public Exception
    {
    public:
        TableNotLinkedException();
    };
}