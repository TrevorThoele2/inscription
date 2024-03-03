#pragma once

#include <typeindex>

#include "Exception.h"

#include "ClassName.h"

namespace Inscription
{
    class PolymorphicTypeNotFound : public Exception
    {
    public:
        PolymorphicTypeNotFound();
        PolymorphicTypeNotFound(const ClassName& className);
        PolymorphicTypeNotFound(const std::type_index& type);
    private:
        static std::string DefaultString();
    };
}