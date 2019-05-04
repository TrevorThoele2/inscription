#pragma once

#include <memory>

#include "TableEntryInherit.h"
#include "StackConstructor.h"
#include "Const.h"

namespace Inscription
{
    template<class ClassT, class T, T ClassT::*var>
    class TableEntryBasic : public TableEntryInherit<ClassT, T>
    {
    private:
        typedef typename RemoveConstTrait<T>::type StoreT;
    public:
        TableEntryBasic* Clone() override;

        T& Get() override;

        void Serialize(Scribe& scribe, ClassT& obj) override;
        void Construct(Scribe& scribe) override;
        void Clear() override;
    private:
        std::unique_ptr<StackConstructor<StoreT>> constructor;
    };

    template<class ClassT, class T, T ClassT::*var>
    TableEntryBasic<ClassT, T, var>* TableEntryBasic<ClassT, T, var>::Clone()
    {
        return new TableEntryBasic();
    }

    template<class ClassT, class T, T ClassT::*var>
    T& TableEntryBasic<ClassT, T, var>::Get()
    {
        return *const_cast<T*>(constructor->Get());
    }

    template<class ClassT, class T, T ClassT::*var>
    void TableEntryBasic<ClassT, T, var>::Serialize(Scribe& scribe, ClassT& obj)
    {
        scribe(RemoveConst(obj.*var));
    }

    template<class ClassT, class T, T ClassT::*var>
    void TableEntryBasic<ClassT, T, var>::Construct(Scribe& scribe)
    {
        constructor.reset(new StackConstructor<StoreT>(scribe));
    }

    template<class ClassT, class T, T ClassT::*var>
    void TableEntryBasic<ClassT, T, var>::Clear()
    {
        constructor.reset();
    }
}