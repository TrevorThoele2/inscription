#pragma once

#include <cstddef>

#ifndef INSCRIPTION_CONTAINER_SIZE_VALUE_T
    #include <cstdint>
#endif

namespace Inscription
{
#ifndef INSCRIPTION_CONTAINER_SIZE_VALUE_T
    typedef std::size_t ContainerSize;
#else
    typedef INSCRIPTION_CONTAINER_SIZE_VALUE_T Value;
#endif
}