#pragma once

#include "GettableTableEntry.h"

#include "Const.h"

namespace Inscription
{
    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    class OwningPointerTableEntry : public GettableTableEntry<ScribeT, ClassT, T>
    {
    private:
        typedef typename RemoveConstTrait<T>::type StoreT;
    public:
        OwningPointerTableEntry();
        OwningPointerTableEntry* Clone() override;

        T& Get() override;

        void Serialize(ScribeT& scribe, ClassT& obj) override;
        void Construct(ScribeT& scribe) override;
        void Clear() override;
    private:
        // Will already be a pointer
        T obj;
    };

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    OwningPointerTableEntry<ScribeT, ClassT, T, var>::OwningPointerTableEntry() : obj(nullptr)
    {}

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    OwningPointerTableEntry<ScribeT, ClassT, T, var>* OwningPointerTableEntry<ScribeT, ClassT, T, var>::Clone()
    {
        return new OwningPointerTableEntry();
    }

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    T& OwningPointerTableEntry<ScribeT, ClassT, T, var>::Get()
    {
        return obj;
    }

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    void OwningPointerTableEntry<ScribeT, ClassT, T, var>::Serialize(ScribeT& scribe, ClassT& obj)
    {
        scribe.OwningPointer(RemoveConst(obj.*var));
    }

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    void OwningPointerTableEntry<ScribeT, ClassT, T, var>::Construct(ScribeT& scribe)
    {
        scribe.LoadOwningPointer(obj);
    }

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    void OwningPointerTableEntry<ScribeT, ClassT, T, var>::Clear()
    {
        obj = nullptr;
    }
}