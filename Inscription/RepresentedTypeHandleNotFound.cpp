#include "RepresentedTypeHandleNotFound.h"

namespace Inscription
{
    RepresentedTypeHandleNotFound::RepresentedTypeHandleNotFound() :
        Exception(
            DefaultString())
    {}

    RepresentedTypeHandleNotFound::RepresentedTypeHandleNotFound(const TypeHandle& typeHandle) :
        Exception(
            DefaultString() +
            " TypeHandle: " + typeHandle + ".")
    {}

    RepresentedTypeHandleNotFound::RepresentedTypeHandleNotFound(const std::type_index& type) :
        Exception(
            DefaultString() +
            " Type: " + type.name() + ".")
    {}

    std::string RepresentedTypeHandleNotFound::DefaultString()
    {
        return std::string(
            "A represented type handle was not found in the PolymorphicManager. "
            "A type may not have been registered polymorphically.");
    }
}