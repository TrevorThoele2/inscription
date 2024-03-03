#pragma once

#include "Exception.h"

namespace Inscription
{
    class FileEncounteredError final : public Exception
    {
    public:
        FileEncounteredError();
    };
}