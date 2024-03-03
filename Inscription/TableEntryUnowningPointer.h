#pragma once

#include "TableEntryInherit.h"

#include "Const.h"

namespace Inscription
{
    template<class ClassT, class T, T ClassT::*var>
    class TableEntryUnowningPointer : public TableEntryInherit<ClassT, T>
    {
    private:
        typedef typename RemoveConstTrait<T>::type StoreT;
    public:
        TableEntryUnowningPointer();
        TableEntryUnowningPointer* Clone() override;

        T& Get() override;

        void Serialize(Scribe& scribe, ClassT& obj) override;
        void Construct(Scribe& scribe) override;
        void Clear() override;
    private:
        // Will already be a pointer
        T obj;
    };

    template<class ClassT, class T, T ClassT::*var>
    TableEntryUnowningPointer<ClassT, T, var>::TableEntryUnowningPointer() : obj(nullptr)
    {}

    template<class ClassT, class T, T ClassT::*var>
    TableEntryUnowningPointer<ClassT, T, var>* TableEntryUnowningPointer<ClassT, T, var>::Clone()
    {
        return new TableEntryUnowningPointer();
    }

    template<class ClassT, class T, T ClassT::*var>
    T& TableEntryUnowningPointer<ClassT, T, var>::Get()
    {
        return obj;
    }

    template<class ClassT, class T, T ClassT::*var>
    void TableEntryUnowningPointer<ClassT, T, var>::Serialize(Scribe& scribe, ClassT& obj)
    {
        scribe.UnowningPointer(RemoveConst(obj.*var));
    }

    template<class ClassT, class T, T ClassT::*var>
    void TableEntryUnowningPointer<ClassT, T, var>::Construct(Scribe& scribe)
    {
        scribe.LoadUnowningPointer(obj);
    }

    template<class ClassT, class T, T ClassT::*var>
    void TableEntryUnowningPointer<ClassT, T, var>::Clear()
    {
        obj = nullptr;
    }
}