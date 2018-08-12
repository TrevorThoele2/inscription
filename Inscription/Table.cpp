
#include "Table.h"

namespace Inscription
{
    TableNotLinkedException::TableNotLinkedException() : Exception("This variable was not linked when the table was serializing it. Link it from the inscripter's serialization function.")
    {}

    TableNotConstructedException::TableNotConstructedException() : Exception("This variable was not constructed before it was serialized. Construct it manually from the inscripter's serialization function.")
    {}

    TableVariableNotHereException::TableVariableNotHereException() : Exception("This variable was not inside of this table.")
    {}
}