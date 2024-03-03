#pragma once

#include "Exception.h"

namespace Inscription
{
    class FileEncounteredError : public Exception
    {
    public:
        FileEncounteredError();
    };
}