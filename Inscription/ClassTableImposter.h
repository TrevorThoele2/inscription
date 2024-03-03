#pragma once

#include <memory>

#include "LinkableTableEntry.h"

#include "ConvertHeap.h"

namespace Inscription
{
    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    class ClassTableImposter : public LinkableTableEntry<ScribeT, ClassT, InterfaceT>
    {
    public:
        ClassTableImposter();
        ClassTableImposter(const ClassTableImposter& arg);
        ClassTableImposter& operator=(const ClassTableImposter &arg) = delete;

        ~ClassTableImposter();

        ClassTableImposter* Clone() override;

        InterfaceT& Get() override;
        void Link(InterfaceT& obj) override;
        void LinkMove(InterfaceT&& obj) override;
    protected:
        void SerializeImpl(ScribeT& scribe, ClassT& obj) override;
        void ConstructImpl(ScribeT& scribe) override;
        bool IsLinked() const override;
    private:
        std::unique_ptr<InterfaceT> interfaceVar;
        bool deleteInterfaceVar;
    };

    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    ClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>::ClassTableImposter() : deleteInterfaceVar(false)
    {}

    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    ClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>::ClassTableImposter(const ClassTableImposter& arg) : deleteInterfaceVar(false)
    {}

    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    ClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>::~ClassTableImposter()
    {
        if (!deleteInterfaceVar)
            interfaceVar.release();
    }

    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    ClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>* ClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>::Clone()
    {
        return new ClassTableImposter(*this);
    }

    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    InterfaceT& ClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>::Get()
    {
        return *interfaceVar;
    }

    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    void ClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>::Link(InterfaceT& obj)
    {
        if (deleteInterfaceVar)
            interfaceVar.reset(&obj);
        else
        {
            interfaceVar.release();
            interfaceVar.reset(&obj);
        }

        deleteInterfaceVar = false;
    }

    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    void ClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>::LinkMove(InterfaceT&& obj)
    {
        Link(obj);
    }

    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    void ClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>::SerializeImpl(ScribeT& scribe, ClassT& obj)
    {
        bool tracking = scribe.TrackObjects(false);

        // Save or load the fileT
        FileT& fileVar(::Inscription::Convert<InterfaceT, FileT>(*interfaceVar));
        scribe(fileVar);
        // Set the interfaceT to the fileT
        Assign(*interfaceVar, fileVar);

        scribe.TrackObjects(tracking);
        scribe.TrackObject(interfaceVar.get());
    }

    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    void ClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>::ConstructImpl(ScribeT& scribe)
    {
        if (!deleteInterfaceVar)
            interfaceVar.release();

        bool tracking = scribe.TrackObjects(false);

        FileT* fileVar;
        ::inscription::Construct(scribe, fileVar);
        interfaceVar.reset(ConvertHeap<FileT, InterfaceT>(*fileVar));
        delete fileVar;

        scribe.TrackObjects(tracking);
        scribe.TrackObject(interfaceVar.get());

        deleteInterfaceVar = true;
    }

    template<class ScribeT, class ClassT, class InterfaceT, class FileT>
    bool ClassTableImposter<ScribeT, ClassT, InterfaceT, FileT>::IsLinked() const
    {
        return interfaceVar != nullptr;
    }
}