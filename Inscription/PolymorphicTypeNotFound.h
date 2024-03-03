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
        explicit PolymorphicTypeNotFound(const ClassName& className);
        explicit PolymorphicTypeNotFound(const std::type_index& type);
    private:
        static std::string DefaultString();
    };
}