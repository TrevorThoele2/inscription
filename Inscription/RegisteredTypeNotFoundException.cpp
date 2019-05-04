
#include "RegisteredTypeNotFoundException.h"

namespace Inscription
{
    RegisteredTypeNotFoundException::RegisteredTypeNotFoundException() : Exception("Class type not registered.")
    {}
}