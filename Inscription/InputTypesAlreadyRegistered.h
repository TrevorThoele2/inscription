#pragma once

#include <vector>
#include "Exception.h"
#include "Type.h"

namespace Inscription
{
    class InputTypesAlreadyRegistered : public Exception
    {
    public:
        InputTypesAlreadyRegistered();
        explicit InputTypesAlreadyRegistered(const std::vector<Type>& types);
    private:
        static std::string DefaultString();
        static std::string Join(const std::vector<Type>& types);
    };
}
