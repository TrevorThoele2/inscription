#pragma once

#include <map>

#include "CompositeScribe.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"
#include "Const.h"

namespace Inscription
{
    class BinaryArchive;

    template<class Key, class T, class Hash, class Allocator>
    class Scribe<std::multimap<Key, T, Hash, Allocator>, BinaryArchive> final :
        public CompositeScribe<std::multimap<Key, T, Hash, Allocator>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<std::multimap<Key, T, Hash, Allocator>, BinaryArchive>;
    public:
        using typename BaseT::ObjectT;
        using typename BaseT::ArchiveT;
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    private:
        static void SaveImplementation(ObjectT& object, ArchiveT& archive);
        static void LoadImplementation(ObjectT& object, ArchiveT& archive);
    };

    template<class Key, class T, class Hash, class Allocator>
    void Scribe<std::multimap<Key, T, Hash, Allocator>, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    template<class Key, class T, class Hash, class Allocator>
    void Scribe<std::multimap<Key, T, Hash, Allocator>, BinaryArchive>::SaveImplementation(ObjectT& object, ArchiveT& archive)
    {
        ContainerSize size(object.size());
        archive(size);
        for (auto loop = object.begin(); loop != object.end(); ++loop)
        {
            archive(loop->first);
            archive(loop->second);
        }
    }

    template<class Key, class T, class Hash, class Allocator>
    void Scribe<std::multimap<Key, T, Hash, Allocator>, BinaryArchive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
    {
        ContainerSize size;
        archive(size);

        object.clear();
        while (size-- > 0)
        {
            ScopeConstructor<typename ObjectT::key_type> key(archive);
            ScopeConstructor<typename ObjectT::mapped_type> mapped(archive);

            auto emplaced = object.emplace(std::move(key.GetMove()), std::move(mapped.GetMove()));
            if (object.count(*key.Get()) == 1)
            {
                archive.ReplaceTrackedObject(*key.Get(), RemoveConst(emplaced->first));
                archive.ReplaceTrackedObject(*mapped.Get(), emplaced->second);
            }
        }
    }
}