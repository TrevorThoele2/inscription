
#include "PolymorphicTypeNotFoundException.h"

namespace Inscription
{
    PolymorphicTypeNotFoundException::PolymorphicTypeNotFoundException() : Exception("Class type not found in the polymorphic handler.")
    {}
}