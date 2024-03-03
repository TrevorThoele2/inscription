#pragma once

#include <map>

#include "ObjectScribe.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"
#include "Const.h"

namespace Inscription
{
    class BinaryArchive;

    template<class Key, class T, class Hash, class Allocator>
    class Scribe<std::map<Key, T, Hash, Allocator>, BinaryArchive> final :
        public ObjectScribe<std::map<Key, T, Hash, Allocator>, BinaryArchive>
    {
    private:
        using BaseT = ObjectScribe<std::map<Key, T, Hash, Allocator>, BinaryArchive>;
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

    template<class Key, class T, class Hash, class Allocator>
    void Scribe<std::map<Key, T, Hash, Allocator>, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    template<class Key, class T, class Hash, class Allocator>
    void Scribe<std::map<Key, T, Hash, Allocator>, BinaryArchive>::SaveImplementation(ObjectT& object, ArchiveT& archive)
    {
        ContainerSize size(object.size());
        archive(size);
        for (auto loop = object.begin(); loop != object.end(); ++loop)
        {
            archive(loop->first);
            archive(loop->second);
        }
    }

    template<class Key, class T, class Hash, class Allocator>
    void Scribe<std::map<Key, T, Hash, Allocator>, BinaryArchive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
    {
        ContainerSize size;
        archive(size);

        object.clear();
        while (size-- > 0)
        {
            ScopeConstructor<typename ObjectT::key_type> key(archive);
            ScopeConstructor<typename ObjectT::mapped_type> mapped(archive);

            auto emplaced = object.emplace(std::move(key.GetMove()), std::move(mapped.GetMove()));
            if (emplaced.second)
            {
                archive.types.AttemptReplaceTrackedObject(*key.Get(), RemoveConst(emplaced.first->first));
                archive.types.AttemptReplaceTrackedObject(*mapped.Get(), emplaced.first->second);
            }
        }
    }

    class JsonArchive;

    template<class Key, class T, class Hash, class Allocator>
    class Scribe<std::map<Key, T, Hash, Allocator>, JsonArchive> final :
        public ObjectScribe<std::map<Key, T, Hash, Allocator>, JsonArchive>
    {
    private:
        using BaseT = ObjectScribe<std::map<Key, T, Hash, Allocator>, JsonArchive>;
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

    template<class Key, class T, class Hash, class Allocator>
    void Scribe<std::map<Key, T, Hash, Allocator>, JsonArchive>::ScrivenImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(name, object, archive);
        else
            LoadImplementation(name, object, archive);
    }

    template<class Key, class T, class Hash, class Allocator>
    void Scribe<std::map<Key, T, Hash, Allocator>, JsonArchive>::SaveImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        auto outputArchive = archive.AsOutput();

        outputArchive->StartList(name);
        for (auto loop = object.begin(); loop != object.end(); ++loop)
        {
            outputArchive->StartList("");
            archive("", loop->first);
            archive("", loop->second);
            outputArchive->EndList();
        }
        outputArchive->EndList();
    }

    template<class Key, class T, class Hash, class Allocator>
    void Scribe<std::map<Key, T, Hash, Allocator>, JsonArchive>::LoadImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        object.clear();

        auto inputArchive = archive.AsInput();

        ContainerSize size = 0;

        inputArchive->StartList(name, size);

        while (size-- > 0)
        {
            ContainerSize innerSize;
            inputArchive->StartList("", innerSize);

            ScopeConstructor<typename ObjectT::key_type> key(archive);
            ScopeConstructor<typename ObjectT::mapped_type> mapped(archive);

            auto emplaced = object.emplace(std::move(key.GetMove()), std::move(mapped.GetMove()));
            if (emplaced.second)
            {
                archive.types.AttemptReplaceTrackedObject(*key.Get(), RemoveConst(emplaced.first->first));
                archive.types.AttemptReplaceTrackedObject(*mapped.Get(), emplaced.first->second);
            }

            inputArchive->EndList();
        }

        inputArchive->EndList();
    }
}