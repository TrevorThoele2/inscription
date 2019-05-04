#pragma once

namespace Inscription
{
    template<class T>
    class Table;

    template<class ClassT>
    class TableEntryBase
    {
    public:
        virtual ~TableEntryBase() = 0;
        virtual TableEntryBase* Clone() = 0;

        virtual void Serialize(Scribe& scribe, ClassT& obj) = 0;
        virtual void Construct(Scribe& scribe) = 0;
    };

    template<class ClassT>
    TableEntryBase<ClassT>::~TableEntryBase()
    {}

    template<class ClassT, class T>
    class TableEntryBaseDerived : public TableEntryBase<ClassT>
    {
    public:
        Table<T> base;
    public:
        TableEntryBaseDerived() = default;
        TableEntryBaseDerived* Clone() override;

        void Serialize(Scribe& scribe, ClassT& obj) override;
        void Construct(Scribe& scribe) override;
    };

    template<class ClassT, class T>
    TableEntryBaseDerived<ClassT, T>* TableEntryBaseDerived<ClassT, T>::Clone()
    {
        return new TableEntryBaseDerived();
    }

    template<class ClassT, class T>
    void TableEntryBaseDerived<ClassT, T>::Serialize(Scribe& scribe, ClassT& obj)
    {
        MakeAndSerializeTable<T>(scribe, obj);
    }

    template<class ClassT, class T>
    void TableEntryBaseDerived<ClassT, T>::Construct(Scribe& scribe)
    {
        base = MakeAndConstructTable<T>(scribe);
    }
}