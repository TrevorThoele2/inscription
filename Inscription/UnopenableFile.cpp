#include "UnopenableFile.h"

namespace Inscription
{
    UnopenableFile::UnopenableFile(const Path& path) : Exception(
        ("The file encountered an error while opening. "
        "The file path: " + path + ".").c_str())
    {}
}