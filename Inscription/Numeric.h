
#pragma once

#include <cstdint>

// To enable common numeric traits, #define INSCRIPTION_COMMON_NUMERICS and then override inscription::NumericTraits for all of the types you need
// The traits must define Size as the type to convert into

namespace Inscription
{
    template<class T>
    struct NumericTraits;
}