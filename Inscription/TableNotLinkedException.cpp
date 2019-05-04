
#include "TableNotLinkedException.h"

namespace Inscription
{
    TableNotLinkedException::TableNotLinkedException() : Exception("This variable was not linked when the table was serializing it. Link it from the inscripter's serialization function.")
    {}
}