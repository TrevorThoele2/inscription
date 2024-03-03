
#include "Exception.h"

namespace inscription
{
    Exception::Exception(const char *message) : runtime_error(message)
    {}
}