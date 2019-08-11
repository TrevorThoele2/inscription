#pragma once

#include <queue>

#include "CompositeScribe.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"

namespace Inscription
{
    class BinaryArchive;

    template<class T, class Container>
    class Scribe<std::queue<T, Container>, BinaryArchive> final :
        public CompositeScribe<std::queue<T, Container>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<std::queue<T, Container>, BinaryArchive>;
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

    template<class T, class Container>
    void Scribe<std::queue<T, Container>, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    template<class T, class Container>
    void Scribe<std::queue<T, Container>, BinaryArchive>::SaveImplementation(ObjectT& object, ArchiveT& archive)
    {
        auto copied = object;

        ContainerSize size(copied.size());
        archive(size);
        while (!copied.empty())
        {
            auto& front = copied.front();
            archive(front);
            copied.pop();
        }
    }

    template<class T, class Container>
    void Scribe<std::queue<T, Container>, BinaryArchive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
    {
        ContainerSize size;
        archive(size);

        while (size-- > 0)
        {
            ScopeConstructor<typename ObjectT::value_type> constructor(archive);
            object.push(std::move(constructor.GetMove()));
            archive.AttemptReplaceTrackedObject(*constructor.Get(), object.back());
        }
    }
}