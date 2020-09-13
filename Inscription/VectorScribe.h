#pragma once

#include <vector>

#include "ObjectScribe.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"

namespace Inscription
{
    class BinaryArchive;

    template<class T, class Allocator>
    class Scribe<std::vector<T, Allocator>, BinaryArchive> final :
        public ObjectScribe<std::vector<T, Allocator>, BinaryArchive>
    {
    private:
        using BaseT = ObjectScribe<std::vector<T, Allocator>, BinaryArchive>;
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
            archive.types.AttemptReplaceTrackedObject(*constructor.Get(), object.back());
        }
    }

    class JsonArchive;

    template<class T, class Allocator>
    class Scribe<std::vector<T, Allocator>, JsonArchive> final :
        public ObjectScribe<std::vector<T, Allocator>, JsonArchive>
    {
    private:
        using BaseT = ObjectScribe<std::vector<T, Allocator>, JsonArchive>;
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

    template<class T, class Allocator>
    void Scribe<std::vector<T, Allocator>, JsonArchive>::ScrivenImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(name, object, archive);
        else
            LoadImplementation(name, object, archive);
    }

    template<class T, class Allocator>
    void Scribe<std::vector<T, Allocator>, JsonArchive>::SaveImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        auto outputArchive = archive.AsOutput();

        outputArchive->StartList(name);
        for (auto loop = object.begin(); loop != object.end(); ++loop)
            archive("", *loop);
        outputArchive->EndList();
    }

    template<class T, class Allocator>
    void Scribe<std::vector<T, Allocator>, JsonArchive>::LoadImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        object.clear();

        auto inputArchive = archive.AsInput();

        ContainerSize size = 0;

        inputArchive->StartList(name, size);

        while (size-- > 0)
        {
            ScopeConstructor<typename ObjectT::value_type> constructor(archive);
            object.push_back(std::move(constructor.GetMove()));
            archive.types.AttemptReplaceTrackedObject(*constructor.Get(), object.front());
        }

        inputArchive->EndList();
    }
}