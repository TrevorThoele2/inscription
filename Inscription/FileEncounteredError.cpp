#include "FileEncounteredError.h"

namespace Inscription
{
    FileEncounteredError::FileEncounteredError(const FilePath& filePath) :
        DetailedException("The file has encountered an error.", { {"FilePath", filePath.string()} })
    {}
}