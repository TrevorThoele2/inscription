#pragma once

#include "Const.h"

#define VERIFY_NON_CONST(T)                                                                 \
static_assert(                                                                              \
    !IsConst<T>::value,                                                                     \
    "A const object cannot be serialized. "                                                 \
    "You must remove const with const_cast, or call inscription::RemoveConst in Const.h.")