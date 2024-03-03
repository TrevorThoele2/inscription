#include "InputTypeNotFound.h"

namespace Inscription
{
    InputTypeNotFound::InputTypeNotFound() :
        Exception(
            DefaultString())
    {}

    InputTypeNotFound::InputTypeNotFound(const Type& type) :
        Exception(
            DefaultString() +
            " Type: " + type + ".")
    {}

    InputTypeNotFound::InputTypeNotFound(const std::type_index& typeIndex) :
        Exception(
            DefaultString() +
            " Type: " + typeIndex.name() + ".")
    {}

    std::string InputTypeNotFound::DefaultString()
    {
        return std::string(
            "An input type was not found in the PolymorphicManager. "
            "A type may not have been registered polymorphically.");
    }
}