#pragma once

#include "TableEntry.h"

namespace Inscription
{
    template<class ScribeT, class ClassT, class T>
    class GettableTableEntry : public TableEntry<ScribeT, ClassT>
    {
    public:
        virtual T& Get() = 0;
    };
}