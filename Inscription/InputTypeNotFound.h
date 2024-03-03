#pragma once

#include <typeindex>

#include "Exception.h"
#include "Type.h"

namespace Inscription
{
    class InputTypeNotFound : public Exception
    {
    public:
        InputTypeNotFound();
        explicit InputTypeNotFound(const Type& type);
        explicit InputTypeNotFound(const std::type_index& typeIndex);
    private:
        static std::string DefaultString();
    };
}