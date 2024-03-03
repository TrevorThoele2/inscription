#include "RegisteredTypeNotFound.h"

namespace Inscription
{
    RegisteredTypeNotFound::RegisteredTypeNotFound() :
        Exception("Class type not registered.")
    {}
}