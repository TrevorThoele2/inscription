#pragma once

#include <set>

#include "ObjectScribe.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"
#include "Const.h"

namespace Inscription
{
    class BinaryArchive;

    template<class Key, class Predicate, class Allocator>
    class Scribe<std::set<Key, Predicate, Allocator>, BinaryArchive> final :
        public ObjectScribe<std::set<Key, Predicate, Allocator>, BinaryArchive>
    {
    private:
        using BaseT = ObjectScribe<std::set<Key, Predicate, Allocator>, BinaryArchive>;
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
                archive.types.AttemptReplaceTrackedObject(*constructor.Get(), RemoveConst(*emplaced.first));
        }
    }

    class JsonArchive;

    template<class Key, class Predicate, class Allocator>
    class Scribe<std::set<Key, Predicate, Allocator>, JsonArchive> final :
        public ObjectScribe<std::set<Key, Predicate, Allocator>, JsonArchive>
    {
    private:
        using BaseT = ObjectScribe<std::set<Key, Predicate, Allocator>, JsonArchive>;
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

    template<class Key, class Predicate, class Allocator>
    void Scribe<std::set<Key, Predicate, Allocator>, JsonArchive>::ScrivenImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(name, object, archive);
        else
            LoadImplementation(name, object, archive);
    }

    template<class Key, class Predicate, class Allocator>
    void Scribe<std::set<Key, Predicate, Allocator>, JsonArchive>::SaveImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        auto outputArchive = archive.AsOutput();

        outputArchive->StartList(name);
        for (auto loop = object.begin(); loop != object.end(); ++loop)
            archive("", *loop);
        outputArchive->EndList();
    }

    template<class Key, class Predicate, class Allocator>
    void Scribe<std::set<Key, Predicate, Allocator>, JsonArchive>::LoadImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        object.clear();

        auto inputArchive = archive.AsInput();

        ContainerSize size = 0;

        inputArchive->StartList(name, size);

        while (size-- > 0)
        {
            ScopeConstructor<typename ObjectT::value_type> constructor(archive);

            auto emplaced = object.emplace(std::move(constructor.GetMove()));
            if (emplaced.second)
                archive.types.AttemptReplaceTrackedObject(*constructor.Get(), RemoveConst(*emplaced.first));
        }

        inputArchive->EndList();
    }
}