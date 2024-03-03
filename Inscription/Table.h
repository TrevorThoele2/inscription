#pragma once

#include "TableBase.h"

namespace Inscription
{
    template<class ScribeT, class ClassT>
    class Table : public TableBase<ScribeT, ClassT>
    {
    public:
        Table() = default;
        Table(const Table& arg);
        Table(Table&& arg);
        Table& operator=(const Table& arg);
        Table& operator=(Table&& arg);
    };

    template<class ScribeT, class ClassT>
    Table<ScribeT, ClassT>::Table(const Table& arg) : TableBase<ScribeT, ClassT>(arg)
    {}

    template<class ScribeT, class ClassT>
    Table<ScribeT, ClassT>::Table(Table&& arg) : TableBase<ScribeT, ClassT>(std::move(arg))
    {}

    template<class ScribeT, class ClassT>
    Table<ScribeT, ClassT>& Table<ScribeT, ClassT>::operator=(const Table& arg)
    {
        TableBase<ScribeT, ClassT>::operator=(arg);
        return *this;
    }

    template<class ScribeT, class ClassT>
    Table<ScribeT, ClassT>& Table<ScribeT, ClassT>::operator=(Table&& arg)
    {
        TableBase<ScribeT, ClassT>::operator=(std::move(arg));
        return *this;
    }
}