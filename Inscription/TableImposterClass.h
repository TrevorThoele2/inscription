#pragma once

#include <memory>

#include "TableLinkable.h"

#include "ConvertHeap.h"

namespace Inscription
{
    template<class ClassT, class InterfaceT, class FileT>
    class TableImposterClass : public TableLinkable<ClassT, InterfaceT>
    {
    public:
        TableImposterClass();
        TableImposterClass(const TableImposterClass& arg);
        TableImposterClass& operator=(const TableImposterClass &arg) = delete;
        ~TableImposterClass();
        TableImposterClass* Clone() override;

        InterfaceT& Get() override;
        void Link(InterfaceT& obj) override;
        void LinkMove(InterfaceT&& obj) override;
    private:
        std::unique_ptr<InterfaceT> interfaceVar;
        bool deleteInterfaceVar;
    private:
        void SerializeImpl(Scribe& scribe, ClassT& obj) override;
        void ConstructImpl(Scribe& scribe) override;
        bool IsValid() const override;
    };

    template<class ClassT, class InterfaceT, class FileT>
    TableImposterClass<ClassT, InterfaceT, FileT>::TableImposterClass() : deleteInterfaceVar(false)
    {}

    template<class ClassT, class InterfaceT, class FileT>
    TableImposterClass<ClassT, InterfaceT, FileT>::TableImposterClass(const TableImposterClass& arg) : deleteInterfaceVar(false)
    {}

    template<class ClassT, class InterfaceT, class FileT>
    TableImposterClass<ClassT, InterfaceT, FileT>::~TableImposterClass()
    {
        if (!deleteInterfaceVar)
            interfaceVar.release();
    }

    template<class ClassT, class InterfaceT, class FileT>
    TableImposterClass<ClassT, InterfaceT, FileT>* TableImposterClass<ClassT, InterfaceT, FileT>::Clone()
    {
        return new TableImposterClass(*this);
    }

    template<class ClassT, class InterfaceT, class FileT>
    InterfaceT& TableImposterClass<ClassT, InterfaceT, FileT>::Get()
    {
        return *interfaceVar;
    }

    template<class ClassT, class InterfaceT, class FileT>
    void TableImposterClass<ClassT, InterfaceT, FileT>::Link(InterfaceT& obj)
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

    template<class ClassT, class InterfaceT, class FileT>
    void TableImposterClass<ClassT, InterfaceT, FileT>::LinkMove(InterfaceT&& obj)
    {
        Link(obj);
    }

    template<class ClassT, class InterfaceT, class FileT>
    void TableImposterClass<ClassT, InterfaceT, FileT>::SerializeImpl(Scribe& scribe, ClassT& obj)
    {
        bool tracking = scribe.TrackObjects(false);

        // Save or load the fileT
        FileT &fileVar(::Inscription::Convert<InterfaceT, FileT>(*interfaceVar));
        scribe(fileVar);
        // Set the interfaceT to the fileT
        Assign(*interfaceVar, fileVar);

        scribe.TrackObjects(tracking);
        scribe.TrackObject(interfaceVar.get());
    }

    template<class ClassT, class InterfaceT, class FileT>
    void TableImposterClass<ClassT, InterfaceT, FileT>::ConstructImpl(Scribe& scribe)
    {
        if (!deleteInterfaceVar)
            interfaceVar.release();

        bool tracking = scribe.TrackObjects(false);

        FileT *fileVar;
        ::inscription::Construct(scribe, fileVar);
        interfaceVar.reset(ConvertHeap<FileT, InterfaceT>(*fileVar));
        delete fileVar;

        scribe.TrackObjects(tracking);
        scribe.TrackObject(interfaceVar.get());

        deleteInterfaceVar = true;
    }

    template<class ClassT, class InterfaceT, class FileT>
    bool TableImposterClass<ClassT, InterfaceT, FileT>::IsValid() const
    {
        return interfaceVar != nullptr;
    }
}