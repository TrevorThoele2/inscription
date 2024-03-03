#pragma once

#include <vector>
#include "Exception.h"
#include "TypeHandle.h"

namespace Inscription
{
    class RepresentedTypeHandlesAlreadyRegistered : public Exception
    {
    public:
        RepresentedTypeHandlesAlreadyRegistered();
        explicit RepresentedTypeHandlesAlreadyRegistered(const std::vector<TypeHandle>& typeHandles);
    private:
        static std::string DefaultString();
        static std::string Join(const std::vector<TypeHandle>& typeHandles);
    };
}
