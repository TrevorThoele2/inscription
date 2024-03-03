#pragma once

#include "Exception.h"
#include "Path.h"

namespace Inscription
{
    class UnopenableFile : public Exception
    {
    public:
        UnopenableFile(const Path& path);
    };
}