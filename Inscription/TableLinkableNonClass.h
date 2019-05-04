#pragma once

#include "TableLinkable.h"

namespace Inscription
{
    template<class ClassT, class T>
    class TableLinkableNonClass : public TableLinkable<ClassT, T>
    {
    public:
        TableLinkableNonClass();
        TableLinkableNonClass* Clone() override;

        T& Get() override;
        void Link(T& obj) override;
        void LinkMove(T&& obj) override;
    private:
        bool valid;
        T var;

        void SerializeImpl(Scribe& scribe, ClassT& obj) override;
        void ConstructImpl(Scribe& scribe) override;
        bool IsValid() const;
    };

    template<class ClassT, class T>
    TableLinkableNonClass<ClassT, T>::TableLinkableNonClass() : valid(false)
    {}

    template<class ClassT, class T>
    TableLinkableNonClass<ClassT, T>* TableLinkableNonClass<ClassT, T>::Clone()
    {
        return new TableLinkableNonClass(*this);
    }

    template<class ClassT, class T>
    T& TableLinkableNonClass<ClassT, T>::Get()
    {
        return var;
    }

    template<class ClassT, class T>
    void TableLinkableNonClass<ClassT, T>::Link(T& obj)
    {
        valid = true;
        var = obj;
    }

    template<class ClassT, class T>
    void TableLinkableNonClass<ClassT, T>::LinkMove(T&& obj)
    {
        valid = true;
        var = obj;
    }

    template<class ClassT, class T>
    void TableLinkableNonClass<ClassT, T>::SerializeImpl(Scribe& scribe, ClassT& obj)
    {
        scribe(var);
    }

    template<class ClassT, class T>
    void TableLinkableNonClass<ClassT, T>::ConstructImpl(Scribe& scribe)
    {
        valid = true;
        scribe(var);
    }

    template<class ClassT, class T>
    bool TableLinkableNonClass<ClassT, T>::IsValid() const
    {
        return valid;
    }
}