#include "PolymorphicTypeNotFound.h"

namespace Inscription
{
    PolymorphicTypeNotFound::PolymorphicTypeNotFound() :
        Exception("Class type not found in the polymorphic handler.")
    {}
}