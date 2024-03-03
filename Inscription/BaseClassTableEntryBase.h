#pragma once

namespace Inscription
{
    template<class ScribeT, class ClassT>
    class BaseClassTableEntryBase
    {
    public:
        virtual ~BaseClassTableEntryBase() = 0;
        virtual BaseClassTableEntryBase* Clone() = 0;

        virtual void Serialize(ScribeT& scribe, ClassT& obj) = 0;
        virtual void Construct(ScribeT& scribe) = 0;
    };

    template<class ScribeT, class ClassT>
    BaseClassTableEntryBase<ScribeT, ClassT>::~BaseClassTableEntryBase()
    {}
}