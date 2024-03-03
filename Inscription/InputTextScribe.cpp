#include "InputTextScribe.h"

namespace Inscription
{
    InputTextScribe::InputTextScribe(const Path& path) : TextScribe(Direction::INPUT), file(path)
    {}
}