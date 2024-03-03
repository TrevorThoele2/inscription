#include "RegisteredTypeNotFound.h"

namespace Inscription
{
    RegisteredTypeNotFound::RegisteredTypeNotFound(const std::type_index& type) :
        Exception((std::string("Class type not registered. Class name expected: ") + type.name() + ".").c_str())
    {}
}