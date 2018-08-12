#pragma once

#include <stdexcept>

namespace Inscription
{
    class Exception : public std::runtime_error
    {
    public:
        Exception(const char *message);
    };
}