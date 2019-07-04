#pragma once

#include "Exception.h"
#include "FilePath.h"

namespace Inscription
{
    class UnopenableFile : public Exception
    {
    public:
        UnopenableFile(const FilePath& path);
    };
}