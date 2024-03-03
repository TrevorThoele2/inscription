#pragma once

#include <stack>

#include "CompositeScribe.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"

namespace Inscription
{
    class BinaryArchive;

    template<class T, class Container>
    class Scribe<std::stack<T, Container>, BinaryArchive> final :
        public CompositeScribe<std::stack<T, Container>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<std::stack<T, Container>, BinaryArchive>;
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

    template<class T, class Container>
    void Scribe<std::stack<T, Container>, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    template<class T, class Container>
    void Scribe<std::stack<T, Container>, BinaryArchive>::SaveImplementation(ObjectT& object, ArchiveT& archive)
    {
        auto copied = object;

        ObjectT reversed;
        while (!copied.empty())
        {
            auto& top = copied.top();
            reversed.push(top);
            copied.pop();
        }

        ContainerSize size(reversed.size());
        archive(size);
        while (!reversed.empty())
        {
            auto& top = reversed.top();
            archive(top);
            reversed.pop();
        }
    }

    template<class T, class Container>
    void Scribe<std::stack<T, Container>, BinaryArchive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
    {
        ContainerSize size;
        archive(size);

        while (size-- > 0)
        {
            ScopeConstructor<typename ObjectT::value_type> constructor(archive);
            object.push(std::move(constructor.GetMove()));
            archive.AttemptReplaceTrackedObject(*constructor.Get(), object.top());
        }
    }
}