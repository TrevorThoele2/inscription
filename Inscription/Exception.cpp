#include "Exception.h"

namespace Inscription
{
    Exception::Exception(const char* message) : runtime_error(message)
    {}

    Exception::Exception(const std::string& message) : runtime_error(message.c_str())
    {}
}