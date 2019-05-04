#pragma once

#include "TableEntryInherit.h"

#include "TableNotLinkedException.h"

namespace Inscription
{
    template<class ClassT, class T>
    class TableLinkable : public TableEntryInherit<ClassT, T>
    {
    public:
        virtual ~TableLinkable() = 0;

        void Serialize(Scribe& scribe, ClassT& obj) override;
        void Construct(Scribe& scribe) override;

        void Clear() override;
        virtual void Link(T& obj) = 0;
        virtual void LinkMove(T&& obj) = 0;
    private:
        virtual void SerializeImpl(Scribe& scribe, ClassT& obj) = 0;
        virtual void ConstructImpl(Scribe& scribe) = 0;
        virtual bool IsValid() const = 0;
    };

    template<class ClassT, class T>
    TableLinkable<ClassT, T>::~TableLinkable()
    {}

    template<class ClassT, class T>
    void TableLinkable<ClassT, T>::Serialize(Scribe& scribe, ClassT& obj)
    {
        if (scribe.IsOutput() && !IsValid())
            throw TableNotLinkedException();

        SerializeImpl(scribe, obj);
    }

    template<class ClassT, class T>
    void TableLinkable<ClassT, T>::Construct(Scribe& scribe)
    {
        ConstructImpl(scribe);
    }

    template<class ClassT, class T>
    void TableLinkable<ClassT, T>::Clear()
    {}
}