#pragma once

#include <vector>

#include "CompositeScribe.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"

namespace Inscription
{
    class BinaryArchive;

    template<class T, class Allocator>
    class Scribe<std::vector<T, Allocator>, BinaryArchive> final :
        public CompositeScribe<std::vector<T, Allocator>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<std::vector<T, Allocator>, BinaryArchive>;
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

    template<class T, class Allocator>
    void Scribe<std::vector<T, Allocator>, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    template<class T, class Allocator>
    void Scribe<std::vector<T, Allocator>, BinaryArchive>::SaveImplementation(ObjectT& object, ArchiveT& archive)
    {
        ContainerSize size(object.size());
        archive(size);
        for (auto loop = object.begin(); loop != object.end(); ++loop)
            archive(*loop);
    }

    template<class T, class Allocator>
    void Scribe<std::vector<T, Allocator>, BinaryArchive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
    {
        ContainerSize size;
        archive(size);

        object.clear();
        while (size-- > 0)
        {
            ScopeConstructor<typename ObjectT::value_type> constructor(archive);
            object.push_back(std::move(constructor.GetMove()));
            archive.AttemptReplaceTrackedObject(*constructor.Get(), object.back());
        }
    }
}