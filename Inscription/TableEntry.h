#pragma once

namespace Inscription
{
    template<class ScribeT, class ClassT>
    class TableEntry
    {
    public:
        virtual ~TableEntry() = 0;
        virtual TableEntry* Clone() = 0;

        virtual void Serialize(ScribeT& scribe, ClassT& obj) = 0;
        virtual void Construct(ScribeT& scribe) = 0;
        virtual void Clear() = 0;
    };

    template<class ScribeT, class ClassT>
    TableEntry<ScribeT, ClassT>::~TableEntry()
    {}
}