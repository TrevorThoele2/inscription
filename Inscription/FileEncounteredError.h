#pragma once

#include <Chroma/DetailedException.h>
#include "FilePath.h"

namespace Inscription
{
    class FileEncounteredError final : public Chroma::DetailedException
    {
    public:
        FileEncounteredError(const FilePath& filePath);
    };
}