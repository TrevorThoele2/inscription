#include "InputTextArchive.h"

namespace Inscription
{
    InputTextArchive::InputTextArchive(const Path& path) : TextArchive(Direction::INPUT), file(path)
    {}
}