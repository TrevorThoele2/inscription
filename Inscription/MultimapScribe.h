#pragma once

#include <map>

#include "TrackingScribeCategory.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"
#include "OutputJsonArchive.h"
#include "InputJsonArchive.h"

namespace Inscription
{
    template<class Key, class T, class Hash, class Allocator>
    class Scribe<std::multimap<Key, T, Hash, Allocator>> final
    {
    public:
        using ObjectT = std::multimap<Key, T, Hash, Allocator>;
    public:
        void Scriven(ObjectT& object, Archive::Binary& archive);
        void Scriven(const std::string& name, ObjectT& object, Archive::Json& archive);
    };

    template<class Key, class T, class Hash, class Allocator>
    void Scribe<std::multimap<Key, T, Hash, Allocator>>::Scriven(ObjectT& object, Archive::Binary& archive)
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
                if (object.count(*key.Get()) == 1)
                {
                    archive.types.AttemptReplaceTrackedObject(*key.Get(), RemoveConst(emplaced->first));
                    archive.types.AttemptReplaceTrackedObject(*mapped.Get(), emplaced->second);
                }
            }
        }
    }

    template<class Key, class T, class Hash, class Allocator>
    void Scribe<std::multimap<Key, T, Hash, Allocator>>::Scriven(const std::string& name, ObjectT& object, Archive::Json& archive)
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
                if (object.count(*key.Get()) == 1)
                {
                    archive.types.AttemptReplaceTrackedObject(*key.Get(), RemoveConst(emplaced->first));
                    archive.types.AttemptReplaceTrackedObject(*mapped.Get(), emplaced->second);
                }

                inputArchive->EndList();
            }

            inputArchive->EndList();
        }
    }

    template<class Key, class T, class Hash, class Allocator, class Archive>
    struct ScribeTraits<std::multimap<Key, T, Hash, Allocator>, Archive>
    {
        using Category = TrackingScribeCategory<std::multimap<Key, T, Hash, Allocator>>;
    };
}