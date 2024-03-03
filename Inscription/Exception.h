#pragma once

#include <stdexcept>

namespace inscription
{
    class Exception : public std::runtime_error
    {
    public:
        Exception(const char *message);
    };
}