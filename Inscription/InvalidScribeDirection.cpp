#include "InvalidScribeDirection.h"

namespace Inscription
{
    InvalidScribeDirection::InvalidScribeDirection() :
        Exception("Attempted operation in the opposite direction of the scribe; saving for an input scribe or loading for an output scribe.")
    {}
}