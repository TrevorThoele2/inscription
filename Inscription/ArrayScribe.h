#pragma once

#include <array>

#include "ObjectScribe.h"

#include "ScopeConstructor.h"

namespace Inscription
{
    class BinaryArchive;

    template<class T, size_t N>
    class Scribe<std::array<T, N>, BinaryArchive> final :
        public ObjectScribe<std::array<T, N>, BinaryArchive>
    {
    private:
        using BaseT = ObjectScribe<std::array<T, N>, BinaryArchive>;
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

    template<class T, size_t N>
    void Scribe<std::array<T, N>, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    template<class T, size_t N>
    void Scribe<std::array<T, N>, BinaryArchive>::SaveImplementation(ObjectT& object, ArchiveT& archive)
    {
        for (auto loop = object.begin(); loop != object.end(); ++loop)
            archive(*loop);
    }

    template<class T, size_t N>
    void Scribe<std::array<T, N>, BinaryArchive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
    {
        std::size_t count = 0;
        while (count < N)
        {
            ScopeConstructor<typename ObjectT::value_type> constructor(archive);
            object[count] = std::move(constructor.GetMove());
            archive.types.AttemptReplaceTrackedObject(*constructor.Get(), object.back());
            ++count;
        }
    }

    class JsonArchive;

    template<class T, size_t N>
    class Scribe<std::array<T, N>, JsonArchive> final :
        public ObjectScribe<std::array<T, N>, JsonArchive>
    {
    private:
        using BaseT = ObjectScribe<std::array<T, N>, JsonArchive>;
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

    template<class T, size_t N>
    void Scribe<std::array<T, N>, JsonArchive>::ScrivenImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(name, object, archive);
        else
            LoadImplementation(name, object, archive);
    }

    template<class T, size_t N>
    void Scribe<std::array<T, N>, JsonArchive>::SaveImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        auto outputArchive = archive.AsOutput();

        outputArchive->StartList(name);
        for (auto loop = object.begin(); loop != object.end(); ++loop)
            archive("", *loop);
        outputArchive->EndList();
    }

    template<class T, size_t N>
    void Scribe<std::array<T, N>, JsonArchive>::LoadImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        auto inputArchive = archive.AsInput();

        ContainerSize size = 0;

        inputArchive->StartList(name, size);

        auto i = 0;
        while (size-- > 0)
        {
            ScopeConstructor<typename ObjectT::value_type> constructor(archive);
            object[i] = std::move(constructor.GetMove());
            archive.types.AttemptReplaceTrackedObject(*constructor.Get(), object.front());
            ++i;
        }

        inputArchive->EndList();
    }
}