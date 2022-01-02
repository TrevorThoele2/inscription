#pragma once

#include "TableDataEntry.h"
#include "BaseTableDataLink.h"

namespace Inscription
{
    template<class Object, class Format>
    struct TableDataBase
    {
        using ObjectT = Object;
        using FormatT = Format;

        template<class T>
        using Entry = TableDataEntry<T>;
        template<class T>
        using Base = BaseTableDataLink<T, ObjectT, FormatT>;
    };
}