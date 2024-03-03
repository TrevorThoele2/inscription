#pragma once

#include <stack>

#include "ObjectScribe.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"

namespace Inscription
{
    class BinaryArchive;

    template<class T, class Container>
    class Scribe<std::stack<T, Container>, BinaryArchive> final :
        public ObjectScribe<std::stack<T, Container>, BinaryArchive>
    {
    private:
        using BaseT = ObjectScribe<std::stack<T, Container>, BinaryArchive>;
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
            archive.types.AttemptReplaceTrackedObject(*constructor.Get(), object.top());
        }
    }

    class JsonArchive;

    template<class T, class Container>
    class Scribe<std::stack<T, Container>, JsonArchive> final :
        public ObjectScribe<std::stack<T, Container>, JsonArchive>
    {
    private:
        using BaseT = ObjectScribe<std::stack<T, Container>, JsonArchive>;
    public:
        using typename BaseT::ObjectT;
        using typename BaseT::ArchiveT;

        using BaseT::Scriven;
    protected:
        void ScrivenImplementation(const std::string& name, ObjectT& object, ArchiveT& archive) override;
    private:
        void SaveImplementation(const std::string& name, ObjectT& object, ArchiveT& archive);
        void LoadImplementation(const std::string& name, ObjectT& object, ArchiveT& archive);
    };

    template<class T, class Container>
    void Scribe<std::stack<T, Container>, JsonArchive>::ScrivenImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(name, object, archive);
        else
            LoadImplementation(name, object, archive);
    }

    template<class T, class Container>
    void Scribe<std::stack<T, Container>, JsonArchive>::SaveImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        auto copied = object;

        ObjectT reversed;
        while (!copied.empty())
        {
            auto& top = copied.top();
            reversed.push(top);
            copied.pop();
        }


        auto outputArchive = archive.AsOutput();

        outputArchive->StartList(name);
        while (!reversed.empty())
        {
            auto& top = reversed.top();
            archive("", top);
            reversed.pop();
        }
        outputArchive->EndList();
    }

    template<class T, class Container>
    void Scribe<std::stack<T, Container>, JsonArchive>::LoadImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        auto inputArchive = archive.AsInput();

        ContainerSize size = 0;

        inputArchive->StartList(name, size);

        while (size-- > 0)
        {
            ScopeConstructor<typename ObjectT::value_type> constructor(archive);
            object.push(std::move(constructor.GetMove()));
            archive.types.AttemptReplaceTrackedObject(*constructor.Get(), object.top());
        }

        inputArchive->EndList();
    }
}