#pragma once

namespace Inscription
{
    class Scribe;

    template<class ClassT>
    class TableEntry
    {
    public:
        virtual ~TableEntry() = 0;
        virtual TableEntry* Clone() = 0;

        virtual void Serialize(Scribe& scribe, ClassT& obj) = 0;
        virtual void Construct(Scribe& scribe) = 0;
        virtual void Clear() = 0;
    };

    template<class ClassT>
    TableEntry<ClassT>::~TableEntry()
    {}
}