#include "TableVariableNotHere.h"

namespace Inscription
{
    TableVariableNotHere::TableVariableNotHere() :
        Exception("This variable was not inside of this table.")
    {}
}