#pragma once

#include <Chroma/DetailedException.h>
#include "FilePath.h"

namespace Inscription::File
{
    class FileEncounteredError final : public Chroma::DetailedException
    {
    public:
        FileEncounteredError(const Path& filePath);
        FileEncounteredError(const std::string& reason, const Path& filePath);
    };
}