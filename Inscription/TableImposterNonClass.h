#pragma once

#include "TableLinkable.h"

namespace Inscription
{
    template<class ClassT, class InterfaceT, class FileT>
    class TableImposterNonClass : public TableLinkable<ClassT, InterfaceT>
    {
    public:
        TableImposterNonClass();
        TableImposterNonClass* Clone() override;

        InterfaceT& Get() override;
        void Link(InterfaceT& obj) override;
        void LinkMove(InterfaceT&& obj) override;
    private:
        bool valid;
        InterfaceT interfaceVar;

        void SerializeImpl(Scribe& scribe, ClassT& obj) override;
        void ConstructImpl(Scribe& scribe) override;
        bool IsValid() const;
    };

    template<class ClassT, class InterfaceT, class FileT>
    TableImposterNonClass<ClassT, InterfaceT, FileT>::TableImposterNonClass() : valid(false)
    {}

    template<class ClassT, class InterfaceT, class FileT>
    TableImposterNonClass<ClassT, InterfaceT, FileT>* TableImposterNonClass<ClassT, InterfaceT, FileT>::Clone()
    {
        return new TableImposterNonClass(*this);
    }

    template<class ClassT, class InterfaceT, class FileT>
    InterfaceT& TableImposterNonClass<ClassT, InterfaceT, FileT>::Get()
    {
        return interfaceVar;
    }

    template<class ClassT, class InterfaceT, class FileT>
    void TableImposterNonClass<ClassT, InterfaceT, FileT>::Link(InterfaceT& obj)
    {
        valid = true;
        interfaceVar = obj;
    }

    template<class ClassT, class InterfaceT, class FileT>
    void TableImposterNonClass<ClassT, InterfaceT, FileT>::LinkMove(InterfaceT&& obj)
    {
        valid = true;
        interfaceVar = obj;
    }

    template<class ClassT, class InterfaceT, class FileT>
    void TableImposterNonClass<ClassT, InterfaceT, FileT>::SerializeImpl(Scribe& scribe, ClassT& obj)
    {
        FileT fileVar = interfaceVar;
        scribe(fileVar);
        interfaceVar = fileVar;
    }

    template<class ClassT, class InterfaceT, class FileT>
    void TableImposterNonClass<ClassT, InterfaceT, FileT>::ConstructImpl(Scribe& scribe)
    {
        valid = true;

        FileT fileVar;
        scribe(fileVar);
        interfaceVar = fileVar;
    }

    template<class ClassT, class InterfaceT, class FileT>
    bool TableImposterNonClass<ClassT, InterfaceT, FileT>::IsValid() const
    {
        return valid;
    }
}