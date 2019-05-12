#include "OutputTextScribe.h"

namespace Inscription
{
    OutputTextScribe::OutputTextScribe(const Path& path, bool append) : TextScribe(Direction::OUTPUT), file(path, append)
    {}
}