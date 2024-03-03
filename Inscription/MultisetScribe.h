#pragma once

#include <set>

#include "CompositeScribe.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"
#include "Const.h"

namespace Inscription
{
    class BinaryArchive;

    template<class Key, class Predicate, class Allocator>
    class Scribe<std::multiset<Key, Predicate, Allocator>, BinaryArchive> final :
        public CompositeScribe<std::multiset<Key, Predicate, Allocator>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<std::multiset<Key, Predicate, Allocator>, BinaryArchive>;
    public:
        using typename BaseT::ObjectT;
        using typename BaseT::ArchiveT;
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    private:
        static void SaveImplementation(ObjectT& object, ArchiveT& archive);
        static void LoadImplementation(ObjectT& object, ArchiveT& archive);
    };

    template<class Key, class Predicate, class Allocator>
    void Scribe<std::multiset<Key, Predicate, Allocator>, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    template<class Key, class Predicate, class Allocator>
    void Scribe<std::multiset<Key, Predicate, Allocator>, BinaryArchive>::SaveImplementation(ObjectT& object, ArchiveT& archive)
    {
        ContainerSize size(object.size());
        archive(size);
        for (auto loop = object.begin(); loop != object.end(); ++loop)
            archive(RemoveConst(*loop));
    }

    template<class Key, class Predicate, class Allocator>
    void Scribe<std::multiset<Key, Predicate, Allocator>, BinaryArchive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
    {
        ContainerSize size;
        archive(size);

        object.clear();
        while (size-- > 0)
        {
            ScopeConstructor<typename ObjectT::value_type> constructor(archive);

            auto emplaced = object.emplace(std::move(constructor.GetMove()));
            if (object.count(*constructor.Get()) == 1)
                archive.ReplaceTrackedObject(*constructor.Get(), RemoveConst(*emplaced));
        }
    }
}