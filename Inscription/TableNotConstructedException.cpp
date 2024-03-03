
#include "TableNotConstructedException.h"

namespace Inscription
{
    TableNotConstructedException::TableNotConstructedException() : Exception("This variable was not constructed before it was serialized. Construct it manually from the inscripter's serialization function.")
    {}
}