#pragma once

#include <stdexcept>
#include <string>

namespace Inscription
{
    class Exception : public std::runtime_error
    {
    public:
        explicit Exception(const char* message);
        explicit Exception(const std::string& message);
    };
}