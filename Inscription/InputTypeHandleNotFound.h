#pragma once

#include <typeindex>

#include "Exception.h"
#include "TypeHandle.h"

namespace Inscription
{
    class InputTypeHandleNotFound : public Exception
    {
    public:
        InputTypeHandleNotFound();
        explicit InputTypeHandleNotFound(const TypeHandle& typeHandle);
        explicit InputTypeHandleNotFound(const std::type_index& type);
    private:
        static std::string DefaultString();
    };
}