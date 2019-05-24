#pragma once

#include <memory>

#include "GettableTableEntry.h"

#include "Const.h"

namespace Inscription
{
    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    class InscripterTableEntry : public GettableTableEntry<ScribeT, ClassT, T>
    {
    private:
        typedef typename RemoveConstTrait<T>::type StoreT;
    public:
        InscripterTableEntry* Clone() override;

        T& Get() override;
        void Serialize(ScribeT& scribe, ClassT& obj) override;
        void Construct(ScribeT& scribe) override;
        void Clear() override;
    private:
        std::unique_ptr<StoreT> stored;
    };

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    InscripterTableEntry<ScribeT, ClassT, T, var>* InscripterTableEntry<ScribeT, ClassT, T, var>::Clone()
    {
        return new InscripterTableEntry();
    }

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    T& InscripterTableEntry<ScribeT, ClassT, T, var>::Get()
    {
        return *const_cast<T*>(stored.get());
    }

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    void InscripterTableEntry<ScribeT, ClassT, T, var>::Serialize(ScribeT& scribe, ClassT& obj)
    {
        MakeAndSerializeTable<T>(scribe, (obj.*var));
    }

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    void InscripterTableEntry<ScribeT, ClassT, T, var>::Construct(ScribeT& scribe)
    {
        T* made = nullptr;
        ::Inscription::Construct<ScribeT, T>(scribe, made);
        stored.reset(made);
    }

    template<class ScribeT, class ClassT, class T, T ClassT::*var>
    void InscripterTableEntry<ScribeT, ClassT, T, var>::Clear()
    {
        stored.reset();
    }
}