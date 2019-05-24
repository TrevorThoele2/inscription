#pragma once

#include <memory>

#include "GettableTableEntry.h"
#include "ScopedConstructor.h"
#include "Const.h"

namespace Inscription
{
    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    class BasicTableEntry : public GettableTableEntry<ScribeT, ClassT, T>
    {
    private:
        typedef typename RemoveConstTrait<T>::type StoreT;
    public:
        BasicTableEntry* Clone() override;

        T& Get() override;

        void Serialize(ScribeT& scribe, ClassT& obj) override;
        void Construct(ScribeT& scribe) override;
        void Clear() override;
    private:
        std::unique_ptr<ScopedConstructor<StoreT>> constructor;
    };

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    BasicTableEntry<ScribeT, ClassT, T, var>* BasicTableEntry<ScribeT, ClassT, T, var>::Clone()
    {
        return new BasicTableEntry();
    }

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    T& BasicTableEntry<ScribeT, ClassT, T, var>::Get()
    {
        return *const_cast<T*>(constructor->Get());
    }

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    void BasicTableEntry<ScribeT, ClassT, T, var>::Serialize(ScribeT& scribe, ClassT& obj)
    {
        scribe(RemoveConst(obj.*var));
    }

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    void BasicTableEntry<ScribeT, ClassT, T, var>::Construct(ScribeT& scribe)
    {
        constructor.reset(new ScopedConstructor<StoreT>(scribe));
    }

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    void BasicTableEntry<ScribeT, ClassT, T, var>::Clear()
    {
        constructor.reset();
    }
}