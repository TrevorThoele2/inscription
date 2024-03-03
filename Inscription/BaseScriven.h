#pragma once

#include "Scribe.h"
#include "Const.h"

namespace Inscription
{
    template<class BaseT, class T, class Archive>
    void BaseScriven(T& object, Archive& archive)
    {
        Scribe<typename RemoveConstTrait<BaseT>::type, Archive> scribe;
        scribe.Scriven(static_cast<BaseT&>(RemoveConst(object)), archive);
    }
}