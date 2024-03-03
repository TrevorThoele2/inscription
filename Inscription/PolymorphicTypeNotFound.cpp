#include "PolymorphicTypeNotFound.h"

namespace Inscription
{
    PolymorphicTypeNotFound::PolymorphicTypeNotFound() :
        Exception(
            "ClassName not found for a polymorphic operation. "
            "The ClassNameResolver for this type is missing or configured incorrectly.")
    {}
}