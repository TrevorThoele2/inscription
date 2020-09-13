#pragma once

#include <unordered_map>

#include "TrackingScribeCategory.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"
#include "OutputJsonArchive.h"
#include "InputJsonArchive.h"

namespace Inscription
{
    class BinaryArchive;

    template<class Key, class T, class Hash, class Predicate, class Allocator>
    class Scribe<std::unordered_map<Key, T, Hash, Predicate, Allocator>> final
    {
    public:
        using ObjectT = std::unordered_map<Key, T, Hash, Predicate, Allocator>;
    public:
        void Scriven(ObjectT& object, BinaryArchive& archive);
        void Scriven(const std::string& name, ObjectT& object, JsonArchive& archive);
    };

    template<class Key, class T, class Hash, class Predicate, class Allocator>
    void Scribe<std::unordered_map<Key, T, Hash, Predicate, Allocator>>::Scriven(ObjectT& object, BinaryArchive& archive)
    {
        if (archive.IsOutput())
        {
            ContainerSize size(object.size());
            archive(size);
            for (auto loop = object.begin(); loop != object.end(); ++loop)
            {
                archive(loop->first);
                archive(loop->second);
            }
        }
        else
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
    }

    template<class Key, class T, class Hash, class Predicate, class Allocator>
    void Scribe<std::unordered_map<Key, T, Hash, Predicate, Allocator>>::Scriven(const std::string& name, ObjectT& object, JsonArchive& archive)
    {
        if (archive.IsOutput())
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
        else
        {
            object.clear();

            auto inputArchive = archive.AsInput();

            auto size = inputArchive->StartList(name);

            while (size-- > 0)
            {
                inputArchive->StartList("");

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

    template<class Key, class T, class Hash, class Predicate, class Allocator, class Archive>
    struct ScribeTraits<std::unordered_map<Key, T, Hash, Predicate, Allocator>, Archive>
    {
        using Category = TrackingScribeCategory<std::unordered_map<Key, T, Hash, Predicate, Allocator>>;
    };
}