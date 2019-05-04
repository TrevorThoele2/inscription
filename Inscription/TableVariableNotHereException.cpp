
#include "TableVariableNotHereException.h"

namespace Inscription
{
    TableVariableNotHereException::TableVariableNotHereException() : Exception("This variable was not inside of this table.")
    {}
}