#include "BaseClassNotInTable.h"

namespace Inscription
{
    BaseClassNotInTable::BaseClassNotInTable(const std::type_index& type) : Exception(
        std::string("A requested base class was not found in the table. ") +
        "Base class: " + type.name() + ".")
    {}
}