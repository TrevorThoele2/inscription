#pragma once

#include <memory>

#include "CompositeScribe.h"

namespace Inscription
{
    class BinaryArchive;

    template<class T, class Deleter>
    class Scribe<std::unique_ptr<T, Deleter>, BinaryArchive> final :
        public CompositeScribe<std::unique_ptr<T, Deleter>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<std::unique_ptr<T, Deleter>, BinaryArchive>;
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

    template<class T, class Deleter>
    void Scribe<std::unique_ptr<T, Deleter>, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    template<class T, class Deleter>
    void Scribe<std::unique_ptr<T, Deleter>, BinaryArchive>::SaveImplementation(ObjectT& object, ArchiveT& archive)
    {
        T* saved = object.get();
        archive(saved);
    }

    template<class T, class Deleter>
    void Scribe<std::unique_ptr<T, Deleter>, BinaryArchive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
    {
        T* loaded = nullptr;
        archive(loaded);
        object.reset(loaded);
    }
}