#pragma once

#include <queue>

#include "ObjectScribe.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"

namespace Inscription
{
    class BinaryArchive;

    template<class T, class Container>
    class Scribe<std::queue<T, Container>, BinaryArchive> final :
        public ObjectScribe<std::queue<T, Container>, BinaryArchive>
    {
    private:
        using BaseT = ObjectScribe<std::queue<T, Container>, BinaryArchive>;
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
            archive.types.AttemptReplaceTrackedObject(*constructor.Get(), object.back());
        }
    }

    class JsonArchive;

    template<class T, class Container>
    class Scribe<std::queue<T, Container>, JsonArchive> final :
        public ObjectScribe<std::queue<T, Container>, JsonArchive>
    {
    private:
        using BaseT = ObjectScribe<std::queue<T, Container>, JsonArchive>;
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
    void Scribe<std::queue<T, Container>, JsonArchive>::ScrivenImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(name, object, archive);
        else
            LoadImplementation(name, object, archive);
    }

    template<class T, class Container>
    void Scribe<std::queue<T, Container>, JsonArchive>::SaveImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        auto outputArchive = archive.AsOutput();

        auto copied = object;

        outputArchive->StartList(name);
        while (!copied.empty())
        {
            auto& front = copied.front();
            archive("", front);
            copied.pop();
        }
        outputArchive->EndList();
    }

    template<class T, class Container>
    void Scribe<std::queue<T, Container>, JsonArchive>::LoadImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        auto inputArchive = archive.AsInput();

        ContainerSize size = 0;

        inputArchive->StartList(name, size);

        while (size-- > 0)
        {
            ScopeConstructor<typename ObjectT::value_type> constructor(archive);
            object.push(std::move(constructor.GetMove()));
            archive.types.AttemptReplaceTrackedObject(*constructor.Get(), object.back());
        }

        inputArchive->EndList();
    }
}