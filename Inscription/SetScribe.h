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
    class Scribe<std::set<Key, Predicate, Allocator>, BinaryArchive> final :
        public CompositeScribe<std::set<Key, Predicate, Allocator>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<std::set<Key, Predicate, Allocator>, BinaryArchive>;
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

    template<class Key, class Predicate, class Allocator>
    void Scribe<std::set<Key, Predicate, Allocator>, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    template<class Key, class Predicate, class Allocator>
    void Scribe<std::set<Key, Predicate, Allocator>, BinaryArchive>::SaveImplementation(ObjectT& object, ArchiveT& archive)
    {
        ContainerSize size(object.size());
        archive(size);
        for (auto loop = object.begin(); loop != object.end(); ++loop)
            archive(*loop);
    }

    template<class Key, class Predicate, class Allocator>
    void Scribe<std::set<Key, Predicate, Allocator>, BinaryArchive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
    {
        ContainerSize size;
        archive(size);

        object.clear();
        while (size-- > 0)
        {
            ScopeConstructor<typename ObjectT::value_type> constructor(archive);

            auto emplaced = object.emplace(std::move(constructor.GetMove()));
            if (emplaced.second)
                archive.AttemptReplaceTrackedObject(*constructor.Get(), RemoveConst(*emplaced.first));
        }
    }
}