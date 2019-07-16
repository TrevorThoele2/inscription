#include "PolymorphicTypeNotFound.h"

namespace Inscription
{
    PolymorphicTypeNotFound::PolymorphicTypeNotFound() :
        Exception(
            DefaultString())
    {}

    PolymorphicTypeNotFound::PolymorphicTypeNotFound(const ClassName& className) :
        Exception(
            DefaultString() +
            "ClassName: " + className + ".")
    {}

    PolymorphicTypeNotFound::PolymorphicTypeNotFound(const std::type_index& type) :
        Exception(
            DefaultString() +
            "Type: " + type.name() + ".")
    {}

    std::string PolymorphicTypeNotFound::DefaultString()
    {
        return std::string(
            "Type was not found in the PolymorphicManager. "
            "The type may not have been registered polymorphically.");
    }
}