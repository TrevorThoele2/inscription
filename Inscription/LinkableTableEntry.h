#pragma once

#include "GettableTableEntry.h"

#include "TableNotLinked.h"

namespace Inscription
{
    template<class ScribeT, class ClassT, class T>
    class LinkableTableEntry : public GettableTableEntry<ClassT, T, ScribeT>
    {
    public:
        virtual ~LinkableTableEntry() = 0;

        void Serialize(ScribeT& scribe, ClassT& obj) override;
        void Construct(ScribeT& scribe) override;

        void Clear() override;
        virtual void Link(T& obj) = 0;
        virtual void LinkMove(T&& obj) = 0;
    protected:
        virtual void SerializeImpl(ScribeT& scribe, ClassT& obj) = 0;
        virtual void ConstructImpl(ScribeT& scribe) = 0;
        virtual bool IsLinked() const = 0;
    };

    template<class ScribeT, class ClassT, class T>
    LinkableTableEntry<ScribeT, ClassT, T>::~LinkableTableEntry()
    {}

    template<class ScribeT, class ClassT, class T>
    void LinkableTableEntry<ScribeT, ClassT, T>::Serialize(ScribeT& scribe, ClassT& obj)
    {
        if (scribe.IsOutput() && !IsLinked())
            throw TableNotLinked();

        SerializeImpl(scribe, obj);
    }

    template<class ScribeT, class ClassT, class T>
    void LinkableTableEntry<ScribeT, ClassT, T>::Construct(ScribeT& scribe)
    {
        ConstructImpl(scribe);
    }

    template<class ScribeT, class ClassT, class T>
    void LinkableTableEntry<ScribeT, ClassT, T>::Clear()
    {}
}