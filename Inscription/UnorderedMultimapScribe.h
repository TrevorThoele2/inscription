#pragma once

#include <unordered_map>

#include "CompositeScribe.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"
#include "Const.h"

namespace Inscription
{
    class BinaryArchive;

    template<class Key, class T, class Hash, class Predicate, class Allocator>
    class Scribe<std::unordered_multimap<Key, T, Hash, Predicate, Allocator>, BinaryArchive> final :
        public CompositeScribe<std::unordered_multimap<Key, T, Hash, Predicate, Allocator>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<std::unordered_multimap<Key, T, Hash, Predicate, Allocator>, BinaryArchive>;
    public:
        using typename BaseT::ObjectT;
        using typename BaseT::ArchiveT;

        using BaseT::Scriven;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    private:
        void SaveImplementation(ObjectT& object, ArchiveT& archive);
        void LoadImplementation(ObjectT& object, ArchiveT& archive);
    };

    template<class Key, class T, class Hash, class Predicate, class Allocator>
    void Scribe<std::unordered_multimap<Key, T, Hash, Predicate, Allocator>, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    template<class Key, class T, class Hash, class Predicate, class Allocator>
    void Scribe<std::unordered_multimap<Key, T, Hash, Predicate, Allocator>, BinaryArchive>::SaveImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        ContainerSize size(object.size());
        archive(size);
        for (auto loop = object.begin(); loop != object.end(); ++loop)
        {
            archive(loop->first);
            archive(loop->second);
        }
    }

    template<class Key, class T, class Hash, class Predicate, class Allocator>
    void Scribe<std::unordered_multimap<Key, T, Hash, Predicate, Allocator>, BinaryArchive>::LoadImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        ContainerSize size;
        archive(size);

        object.clear();
        while (size-- > 0)
        {
            ScopeConstructor<typename ObjectT::key_type> key(archive);
            ScopeConstructor<typename ObjectT::mapped_type> mapped(archive);

            auto emplaced = object.emplace(std::move(*key.Get()), std::move(mapped.GetMove()));
            if (object.count(*key.Get()) == 1)
            {
                archive.AttemptReplaceTrackedObject(*key.Get(), RemoveConst(emplaced->first));
                archive.AttemptReplaceTrackedObject(*mapped.Get(), emplaced->second);
            }
        }
    }
}