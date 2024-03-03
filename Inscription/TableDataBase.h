#pragma once

#include "TableDataEntry.h"
#include "BaseTableDataLink.h"

namespace Inscription
{
    template<class Object, class Archive>
    struct TableDataBase
    {
        using ObjectT = Object;
        using ArchiveT = Archive;

        template<class T>
        using Entry = TableDataEntry<T>;
        template<class T>
        using Base = BaseTableDataLink<T, ObjectT, ArchiveT>;
    };
}