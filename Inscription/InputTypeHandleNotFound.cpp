#include "InputTypeHandleNotFound.h"

namespace Inscription
{
    InputTypeHandleNotFound::InputTypeHandleNotFound() :
        Exception(
            DefaultString())
    {}

    InputTypeHandleNotFound::InputTypeHandleNotFound(const TypeHandle& typeHandle) :
        Exception(
            DefaultString() +
            " TypeHandle: " + typeHandle + ".")
    {}

    InputTypeHandleNotFound::InputTypeHandleNotFound(const std::type_index& type) :
        Exception(
            DefaultString() +
            " Type: " + type.name() + ".")
    {}

    std::string InputTypeHandleNotFound::DefaultString()
    {
        return std::string(
            "An input type handle was not found in the PolymorphicManager. "
            "A type may not have been registered polymorphically.");
    }
}