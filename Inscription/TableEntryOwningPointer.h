#pragma once

#include "TableEntryInherit.h"

#include "Const.h"

namespace Inscription
{
    template<class ClassT, class T, T ClassT::*var>
    class TableEntryOwningPointer : public TableEntryInherit<ClassT, T>
    {
    private:
        typedef typename RemoveConstTrait<T>::type StoreT;
    public:
        TableEntryOwningPointer();
        TableEntryOwningPointer* Clone() override;

        T& Get() override;

        void Serialize(Scribe& scribe, ClassT& obj) override;
        void Construct(Scribe& scribe) override;
        void Clear() override;
    private:
        // Will already be a pointer
        T obj;
    };

    template<class ClassT, class T, T ClassT::*var>
    TableEntryOwningPointer<ClassT, T, var>::TableEntryOwningPointer() : obj(nullptr)
    {}

    template<class ClassT, class T, T ClassT::*var>
    TableEntryOwningPointer<ClassT, T, var>* TableEntryOwningPointer<ClassT, T, var>::Clone()
    {
        return new TableEntryOwningPointer();
    }

    template<class ClassT, class T, T ClassT::*var>
    T& TableEntryOwningPointer<ClassT, T, var>::Get()
    {
        return obj;
    }

    template<class ClassT, class T, T ClassT::*var>
    void TableEntryOwningPointer<ClassT, T, var>::Serialize(Scribe& scribe, ClassT& obj)
    {
        scribe.OwningPointer(RemoveConst(obj.*var));
    }

    template<class ClassT, class T, T ClassT::*var>
    void TableEntryOwningPointer<ClassT, T, var>::Construct(Scribe& scribe)
    {
        scribe.LoadOwningPointer(obj);
    }

    template<class ClassT, class T, T ClassT::*var>
    void TableEntryOwningPointer<ClassT, T, var>::Clear()
    {
        obj = nullptr;
    }
}