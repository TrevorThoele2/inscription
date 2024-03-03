#pragma once

#include <array>

#include "Composite.h"

#include "ScopeConstructor.h"

namespace Inscription
{
    class BinaryArchive;

    template<class T, size_t N>
    class Scribe<std::array<T, N>, BinaryArchive> final :
        public CompositeScribe<std::array<T, N>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<std::array<T, N>, BinaryArchive>;
    public:
        using typename BaseT::ObjectT;
        using typename BaseT::ArchiveT;
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    private:
        static void SaveImplementation(ObjectT& object, ArchiveT& archive);
        static void LoadImplementation(ObjectT& object, ArchiveT& archive);
    };

    template<class T, size_t N>
    void Scribe<std::array<T, N>, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    template<class T, size_t N>
    void Scribe<std::array<T, N>, BinaryArchive>::SaveImplementation(ObjectT& object, ArchiveT& archive)
    {
        for (auto loop = object.begin(); loop != object.end(); ++loop)
            archive(*loop);
    }

    template<class T, size_t N>
    void Scribe<std::array<T, N>, BinaryArchive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
    {
        std::size_t count = 0;
        while (count < N)
        {
            ScopeConstructor<typename ObjectT::value_type> constructor(archive);
            object[count] = std::move(constructor.GetMove());
            archive.ReplaceTrackedObject(*constructor.Get(), object.back());
            ++count;
        }
    }
}