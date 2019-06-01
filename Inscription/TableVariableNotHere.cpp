#include "TableVariableNotHere.h"

namespace Inscription
{
    TableVariableNotHere::TableVariableNotHere(const std::string& name) : Exception(
        "A requested variable was not found in the table. "
        "Variable name: " + name + ".")
    {}
}