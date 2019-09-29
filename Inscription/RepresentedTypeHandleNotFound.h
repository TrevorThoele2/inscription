#pragma once

#include <typeindex>

#include "Exception.h"
#include "TypeHandle.h"

namespace Inscription
{
    class RepresentedTypeHandleNotFound : public Exception
    {
    public:
        RepresentedTypeHandleNotFound();
        explicit RepresentedTypeHandleNotFound(const TypeHandle& typeHandle);
        explicit RepresentedTypeHandleNotFound(const std::type_index& type);
    private:
        static std::string DefaultString();
    };
}