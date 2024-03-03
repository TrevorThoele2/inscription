#include "FileEncounteredError.h"

namespace Inscription::File
{
    FileEncounteredError::FileEncounteredError(const Path& filePath) :
        DetailedException(
            "The file has encountered an error.",
            {
                { "FilePath", filePath.string() }
            })
    {}

    FileEncounteredError::FileEncounteredError(const std::string& reason, const Path& filePath) :
        DetailedException(
            "The file has encountered an error.",
            {
                { "Reason", reason },
                { "FilePath", filePath.string() }
            })
    {}
}