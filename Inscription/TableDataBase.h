#pragma once

#include "BaseTableDataEntry.h"

namespace Inscription
{
    template<class Object, class Archive>
    struct TableDataBase
    {
        using ObjectT = Object;
        using ArchiveT = Archive;

        template<class T>
        using Base = BaseTableDataEntry<T, ObjectT, ArchiveT>;
    };
}