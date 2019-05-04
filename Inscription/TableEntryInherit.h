#pragma once

#include "TableEntry.h"

namespace Inscription
{
    template<class ClassT, class T>
    class TableEntryInherit : public TableEntry<ClassT>
    {
    public:
        virtual T& Get() = 0;
    };
}