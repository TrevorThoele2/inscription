#pragma once

#include <cstddef>

#ifndef INSCRIPTION_CONTAINER_SIZE_VALUE_T
    #include <cstdint>
#endif

namespace Inscription
{
#ifndef INSCRIPTION_CONTAINER_SIZE_VALUE_T
    using ContainerSize = std::size_t;
#else
    using Value = INSCRIPTION_CONTAINER_SIZE_VALUE_T;
#endif
}