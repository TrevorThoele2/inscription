#include "TableNotLinked.h"

namespace Inscription
{
    TableNotLinked::TableNotLinked() :
        Exception("This variable was not linked when the table was serializing it. Link it from the inscripter's serialization function.")
    {}
}