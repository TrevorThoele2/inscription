#pragma once

#include <memory>

#include "Composite.h"

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
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    private:
        static void SaveImplementation(ObjectT& object, ArchiveT& archive);
        static void LoadImplementation(ObjectT& object, ArchiveT& archive);
    };

    template<class T, class Deleter>
    void Scribe<std::unique_ptr<T, Deleter>, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
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