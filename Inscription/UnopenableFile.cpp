#include "UnopenableFile.h"

namespace Inscription
{
    UnopenableFile::UnopenableFile(const FilePath& path) : Exception(
        ("The file encountered an error while opening. "
        "The file path: " + path.GetValue() + ".").c_str())
    {}
}