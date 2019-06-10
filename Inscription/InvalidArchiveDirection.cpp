#include "InvalidArchiveDirection.h"

namespace Inscription
{
    InvalidArchiveDirection::InvalidArchiveDirection() :
        Exception("Attempted operation in the opposite direction of the archive; saving for an input archive or loading for an output archive.")
    {}
}