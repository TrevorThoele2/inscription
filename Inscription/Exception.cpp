#include "Exception.h"

namespace Inscription
{
    Exception::Exception(const char* message) : runtime_error(message)
    {}
}