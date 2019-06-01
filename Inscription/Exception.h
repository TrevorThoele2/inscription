#pragma once

#include <stdexcept>
#include <string>

namespace Inscription
{
    class Exception : public std::runtime_error
    {
    public:
        Exception(const char* message);
        Exception(const std::string& message);
    };
}