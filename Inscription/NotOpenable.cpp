#include "NotOpenable.h"

namespace Inscription
{
    NotOpenable::NotOpenable(const FilePath& path) : Exception(
        ("The file encountered an error while opening. "
        "The file path: " + path.string() + ".").c_str())
    {}
}