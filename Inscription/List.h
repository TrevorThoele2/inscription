#pragma once

#include <list>

#include "Composite.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"

namespace Inscription
{
    class BinaryArchive;

    template<class T, class Allocator>
    class Scribe<std::list<T, Allocator>, BinaryArchive> final :
        public CompositeScribe<std::list<T, Allocator>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<std::list<T, Allocator>, BinaryArchive>;
    public:
        using typename BaseT::ObjectT;
        using typename BaseT::ArchiveT;
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    private:
        static void SaveImplementation(ObjectT& object, ArchiveT& archive);
        static void LoadImplementation(ObjectT& object, ArchiveT& archive);
    };

    template<class T, class Allocator>
    void Scribe<std::list<T, Allocator>, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    template<class T, class Allocator>
    void Scribe<std::list<T, Allocator>, BinaryArchive>::SaveImplementation(ObjectT& object, ArchiveT& archive)
    {
        ContainerSize size(std::distance(object.begin(), object.end()));
        archive(size);
        for (auto loop = object.begin(); loop != object.end(); ++loop)
            archive(*loop);
    }

    template<class T, class Allocator>
    void Scribe<std::list<T, Allocator>, BinaryArchive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
    {
        ContainerSize size;
        archive(size);

        object.clear();
        while (size-- > 0)
        {
            ScopeConstructor<typename ObjectT::value_type> constructor(archive);
            object.push_back(std::move(constructor.GetMove()));
            archive.ReplaceTrackedObject(*constructor.Get(), object.front());
        }
    }
}