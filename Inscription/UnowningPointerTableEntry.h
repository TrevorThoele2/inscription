#pragma once

#include "GettableTableEntry.h"

#include "Const.h"

namespace Inscription
{
    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    class UnowningPointerTableEntry : public GettableTableEntry<ScribeT, ClassT, T>
    {
    private:
        typedef typename RemoveConstTrait<T>::type StoreT;
    public:
        UnowningPointerTableEntry();
        UnowningPointerTableEntry* Clone() override;

        T& Get() override;

        void Serialize(ScribeT& scribe, ClassT& obj) override;
        void Construct(ScribeT& scribe) override;
        void Clear() override;
    private:
        // Will already be a pointer
        T obj;
    };

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    UnowningPointerTableEntry<ScribeT, ClassT, T, var>::UnowningPointerTableEntry() : obj(nullptr)
    {}

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    UnowningPointerTableEntry<ScribeT, ClassT, T, var>* UnowningPointerTableEntry<ScribeT, ClassT, T, var>::Clone()
    {
        return new UnowningPointerTableEntry();
    }

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    T& UnowningPointerTableEntry<ScribeT, ClassT, T, var>::Get()
    {
        return obj;
    }

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    void UnowningPointerTableEntry<ScribeT, ClassT, T, var>::Serialize(ScribeT& scribe, ClassT& obj)
    {
        scribe.UnowningPointer(RemoveConst(obj.*var));
    }

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    void UnowningPointerTableEntry<ScribeT, ClassT, T, var>::Construct(ScribeT& scribe)
    {
        scribe.LoadUnowningPointer(obj);
    }

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    void UnowningPointerTableEntry<ScribeT, ClassT, T, var>::Clear()
    {
        obj = nullptr;
    }
}