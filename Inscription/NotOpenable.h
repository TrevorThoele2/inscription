#pragma once

#include "Exception.h"
#include "FilePath.h"

namespace Inscription
{
    class NotOpenable final : public Exception
    {
    public:
        explicit NotOpenable(const FilePath& path);
    };
}