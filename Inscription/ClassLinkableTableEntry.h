#pragma once

#include "LinkableTableEntry.h"

namespace Inscription
{
    template<class ScribeT, class ClassT, class T>
    class ClassLinkableTableEntry : public LinkableTableEntry<ScribeT, ClassT, T>
    {
    public:
        ClassLinkableTableEntry();
        ClassLinkableTableEntry* Clone() override;

        T& Get() override;
        void Link(T& obj) override;
        void LinkMove(T&& obj) override;
    protected:
        void SerializeImpl(Scribe& scribe, ClassT& obj) override;
        void ConstructImpl(Scribe& scribe) override;
        bool IsLinked() const override;
    private:
        T *var;
    };

    template<class ScribeT, class ClassT, class T>
    ClassLinkableTableEntry<ScribeT, ClassT, T>::ClassLinkableTableEntry() : var(nullptr)
    {}

    template<class ScribeT, class ClassT, class T>
    ClassLinkableTableEntry<ScribeT, ClassT, T>* ClassLinkableTableEntry<ScribeT, ClassT, T>::Clone()
    {
        return new ClassLinkableTableEntry(*this);
    }

    template<class ScribeT, class ClassT, class T>
    T& ClassLinkableTableEntry<ScribeT, ClassT, T>::Get()
    {
        return *var;
    }

    template<class ScribeT, class ClassT, class T>
    void ClassLinkableTableEntry<ScribeT, ClassT, T>::Link(T& obj)
    {
        var = &obj;
    }

    template<class ScribeT, class ClassT, class T>
    void ClassLinkableTableEntry<ScribeT, ClassT, T>::LinkMove(T&& obj)
    {
        var = &obj;
    }

    template<class ScribeT, class ClassT, class T>
    void ClassLinkableTableEntry<ScribeT, ClassT, T>::SerializeImpl(Scribe& scribe, ClassT& obj)
    {
        scribe(*var);
    }

    template<class ScribeT, class ClassT, class T>
    void ClassLinkableTableEntry<ScribeT, ClassT, T>::ConstructImpl(Scribe& scribe)
    {
        if (var)
            scribe(*var);
        else
            ::Inscription::Construct(scribe, var);
    }

    template<class ScribeT, class ClassT, class T>
    bool ClassLinkableTableEntry<ScribeT, ClassT, T>::IsLinked() const
    {
        return var != nullptr;
    }
}