#pragma once

#include "BaseClassTableEntryBase.h"

namespace Inscription
{
    template<class ScribeT, class ClassT>
    class Table;

    template<class ScribeT, class ClassT, class BaseT>
    class BaseClassTableEntry : public BaseClassTableEntryBase<ScribeT, ClassT>
    {
    public:
        Table<ScribeT, BaseT> base;
    public:
        BaseClassTableEntry() = default;
        BaseClassTableEntry* Clone() override;

        void Serialize(ScribeT& scribe, ClassT& obj) override;
        void Construct(ScribeT& scribe) override;
    };

    template<class ScribeT, class ClassT, class BaseT>
    BaseClassTableEntry<ScribeT, ClassT, BaseT>* BaseClassTableEntry<ScribeT, ClassT, BaseT>::Clone()
    {
        return new BaseClassTableEntry();
    }

    template<class ScribeT, class ClassT, class BaseT>
    void BaseClassTableEntry<ScribeT, ClassT, BaseT>::Serialize(ScribeT& scribe, ClassT& obj)
    {
        MakeAndSerializeTable<BaseT>(scribe, obj);
    }

    template<class ScribeT, class ClassT, class BaseT>
    void BaseClassTableEntry<ScribeT, ClassT, BaseT>::Construct(ScribeT& scribe)
    {
        base = MakeAndConstructTable<BaseT>(scribe);
    }
}