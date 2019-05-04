#pragma once

#include "TableLinkable.h"

namespace Inscription
{
    template<class ClassT, class T>
    class TableLinkableClass : public TableLinkable<ClassT, T>
    {
    public:
        TableLinkableClass();
        TableLinkableClass* Clone() override;

        T& Get() override;
        void Link(T& obj) override;
        void LinkMove(T&& obj) override;
    private:
        T *var;
        void SerializeImpl(Scribe& scribe, ClassT& obj) override;
        void ConstructImpl(Scribe& scribe) override;
        bool IsValid() const override;
    };

    template<class ClassT, class T>
    TableLinkableClass<ClassT, T>::TableLinkableClass() : var(nullptr)
    {}

    template<class ClassT, class T>
    TableLinkableClass<ClassT, T>* TableLinkableClass<ClassT, T>::Clone()
    {
        return new TableLinkableClass(*this);
    }

    template<class ClassT, class T>
    T& TableLinkableClass<ClassT, T>::Get()
    {
        return *var;
    }

    template<class ClassT, class T>
    void TableLinkableClass<ClassT, T>::Link(T& obj)
    {
        var = &obj;
    }

    template<class ClassT, class T>
    void TableLinkableClass<ClassT, T>::LinkMove(T&& obj)
    {
        var = &obj;
    }

    template<class ClassT, class T>
    void TableLinkableClass<ClassT, T>::SerializeImpl(Scribe& scribe, ClassT& obj)
    {
        scribe(*var);
    }

    template<class ClassT, class T>
    void TableLinkableClass<ClassT, T>::ConstructImpl(Scribe& scribe)
    {
        if (var)
            scribe(*var);
        else
            ::inscription::Construct(scribe, var);
    }

    template<class ClassT, class T>
    bool TableLinkableClass<ClassT, T>::IsValid() const
    {
        return var != nullptr;
    }
}