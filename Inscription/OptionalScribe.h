#pragma once

#include "Optional.h"
#include "CompositeScribe.h"

#include "ScopeConstructor.h"

namespace Inscription
{
    class BinaryArchive;

    template<class T>
    class Scribe<Optional<T>, BinaryArchive> final :
        public CompositeScribe<Optional<T>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<Optional<T>, BinaryArchive>;
    public:
        using typename BaseT::ObjectT;
        using typename BaseT::ArchiveT;

        using BaseT::Scriven;
        using BaseT::Construct;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
        {
            DoBasicConstruction(storage, archive);
        }

        using BaseT::DoBasicConstruction;
    private:
        void SaveImplementation(ObjectT& object, ArchiveT& archive);
        void LoadImplementation(ObjectT& object, ArchiveT& archive);
    };

    template<class T>
    void Scribe<Optional<T>, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    template<class T>
    void Scribe<Optional<T>, BinaryArchive>::SaveImplementation(ObjectT& object, ArchiveT& archive)
    {
        auto isValid = object.IsValid();
        archive(isValid);

        if (isValid)
            archive(object.Get());
    }

    template<class T>
    void Scribe<Optional<T>, BinaryArchive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
    {
        bool isValid;
        archive(isValid);

        if (isValid)
        {
            ::Inscription::ScopeConstructor<T> constructor(archive);
            object.Set(std::move(constructor.GetMove()));
        }
        else
            object.Reset();
    }
}