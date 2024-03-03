#pragma once

#include "LinkableTableEntry.h"

namespace Inscription
{
    template<class ScribeT, class ClassT, class T>
    class NonClassLinkableTableEntry : public LinkableTableEntry<ScribeT, ClassT, T>
    {
    public:
        NonClassLinkableTableEntry();
        NonClassLinkableTableEntry* Clone() override;

        T& Get() override;
        void Link(T& obj) override;
        void LinkMove(T&& obj) override;
    protected:
        void SerializeImpl(ScribeT& scribe, ClassT& obj) override;
        void ConstructImpl(ScribeT& scribe) override;
        bool IsLinked() const override;
    private:
        bool linked;
        T var;
    };

    template<class ScribeT, class ClassT, class T>
    NonClassLinkableTableEntry<ScribeT, ClassT, T>::NonClassLinkableTableEntry() : linked(false)
    {}

    template<class ScribeT, class ClassT, class T>
    NonClassLinkableTableEntry<ScribeT, ClassT, T>* NonClassLinkableTableEntry<ScribeT, ClassT, T>::Clone()
    {
        return new NonClassLinkableTableEntry(*this);
    }

    template<class ScribeT, class ClassT, class T>
    T& NonClassLinkableTableEntry<ScribeT, ClassT, T>::Get()
    {
        return var;
    }

    template<class ScribeT, class ClassT, class T>
    void NonClassLinkableTableEntry<ScribeT, ClassT, T>::Link(T& obj)
    {
        linked = true;
        var = obj;
    }

    template<class ScribeT, class ClassT, class T>
    void NonClassLinkableTableEntry<ScribeT, ClassT, T>::LinkMove(T&& obj)
    {
        linked = true;
        var = obj;
    }

    template<class ScribeT, class ClassT, class T>
    void NonClassLinkableTableEntry<ScribeT, ClassT, T>::SerializeImpl(ScribeT& scribe, ClassT& obj)
    {
        scribe(var);
    }

    template<class ScribeT, class ClassT, class T>
    void NonClassLinkableTableEntry<ScribeT, ClassT, T>::ConstructImpl(ScribeT& scribe)
    {
        linked = true;
        scribe(var);
    }

    template<class ScribeT, class ClassT, class T>
    bool NonClassLinkableTableEntry<ScribeT, ClassT, T>::IsLinked() const
    {
        return linked;
    }
}