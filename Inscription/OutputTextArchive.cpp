#include "OutputTextArchive.h"

namespace Inscription
{
    OutputTextArchive::OutputTextArchive(const Path& path, bool append) : TextArchive(Direction::OUTPUT), file(path, append)
    {}
}