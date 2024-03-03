#pragma once

#include <vector>
#include "Exception.h"
#include "TypeHandle.h"

namespace Inscription
{
    class InputTypeHandlesAlreadyRegistered : public Exception
    {
    public:
        InputTypeHandlesAlreadyRegistered();
        explicit InputTypeHandlesAlreadyRegistered(const std::vector<TypeHandle>& typeHandles);
    private:
        static std::string DefaultString();
        static std::string Join(const std::vector<TypeHandle>& typeHandles);
    };
}
