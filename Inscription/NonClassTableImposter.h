#pragma once

#include "LinkableTableEntry.h"

namespace Inscription
{
    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    class NonClassTableImposter : public LinkableTableEntry<ClassT, InterfaceT, ScribeT>
    {
    public:
        NonClassTableImposter();
        NonClassTableImposter* Clone() override;

        InterfaceT& Get() override;
        void Link(InterfaceT& obj) override;
        void LinkMove(InterfaceT&& obj) override;
    protected:
        void SerializeImpl(ScribeT& scribe, ClassT& obj) override;
        void ConstructImpl(ScribeT& scribe) override;
        bool IsLinked() const override;
    private:
        bool linked;
        InterfaceT interfaceVar;
    };

    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    NonClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>::NonClassTableImposter() : linked(false)
    {}

    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    NonClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>* NonClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>::Clone()
    {
        return new NonClassTableImposter(*this);
    }

    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    InterfaceT& NonClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>::Get()
    {
        return interfaceVar;
    }

    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    void NonClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>::Link(InterfaceT& obj)
    {
        linked = true;
        interfaceVar = obj;
    }

    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    void NonClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>::LinkMove(InterfaceT&& obj)
    {
        linked = true;
        interfaceVar = obj;
    }

    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    void NonClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>::SerializeImpl(ScribeT& scribe, ClassT& obj)
    {
        FileT fileVar = interfaceVar;
        scribe(fileVar);
        interfaceVar = fileVar;
    }

    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    void NonClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>::ConstructImpl(ScribeT& scribe)
    {
        linked = true;

        FileT fileVar;
        scribe(fileVar);
        interfaceVar = fileVar;
    }

    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    bool NonClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>::IsLinked() const
    {
        return linked;
    }
}