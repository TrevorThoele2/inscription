#pragma once

#include <memory>

#include "TableEntryInherit.h"

#include "Const.h"

namespace Inscription
{
    template<class ClassT, class T, T ClassT::*var>
    class TableEntryInscripter : public TableEntryInherit<ClassT, T>
    {
    private:
        typedef typename RemoveConstTrait<T>::type StoreT;
    public:
        TableEntryInscripter* Clone() override;

        T& Get() override;
        void Serialize(Scribe& scribe, ClassT& obj) override;
        void Construct(Scribe& scribe) override;
        void Clear() override;
    private:
        std::unique_ptr<StoreT> stored;
    };

    template<class ClassT, class T, T ClassT::*var>
    TableEntryInscripter<ClassT, T, var>* TableEntryInscripter<ClassT, T, var>::Clone()
    {
        return new TableEntryInscripter();
    }

    template<class ClassT, class T, T ClassT::*var>
    T& TableEntryInscripter<ClassT, T, var>::Get()
    {
        return *const_cast<T*>(stored.get());
    }

    template<class ClassT, class T, T ClassT::*var>
    void TableEntryInscripter<ClassT, T, var>::Serialize(Scribe& scribe, ClassT& obj)
    {
        MakeAndSerializeTable<T>(scribe, (obj.*var));
    }

    template<class ClassT, class T, T ClassT::*var>
    void TableEntryInscripter<ClassT, T, var>::Construct(Scribe& scribe)
    {
        T *made = nullptr;
        ::Inscription::Construct<T>(scribe, made);
        stored.reset(made);
    }

    template<class ClassT, class T, T ClassT::*var>
    void TableEntryInscripter<ClassT, T, var>::Clear()
    {
        stored.reset();
    }
}