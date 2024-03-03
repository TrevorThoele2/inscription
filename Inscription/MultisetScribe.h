#pragma once

#include <set>

#include "TrackingScribeCategory.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"
#include "OutputJsonArchive.h"
#include "InputJsonArchive.h"

namespace Inscription
{
    template<class Key, class Predicate, class Allocator>
    class Scribe<std::multiset<Key, Predicate, Allocator>> final
    {
    public:
        using ObjectT = std::multiset<Key, Predicate, Allocator>;
    public:
        void Scriven(ObjectT& object, Archive::Binary& archive);
        void Scriven(const std::string& name, ObjectT& object, Archive::Json& archive);
    };

    template<class Key, class Predicate, class Allocator>
    void Scribe<std::multiset<Key, Predicate, Allocator>>::Scriven(ObjectT& object, Archive::Binary& archive)
    {
        if (archive.IsOutput())
        {
            ContainerSize size(object.size());
            archive(size);
            for (auto loop = object.begin(); loop != object.end(); ++loop)
                archive(RemoveConst(*loop));
        }
        else
        {
            ContainerSize size;
            archive(size);

            object.clear();
            while (size-- > 0)
            {
                ScopeConstructor<typename ObjectT::value_type> constructor(archive);

                auto emplaced = object.emplace(std::move(constructor.GetMove()));
                if (object.count(*constructor.Get()) == 1)
                    archive.types.AttemptReplaceTrackedObject(*constructor.Get(), RemoveConst(*emplaced));
            }
        }
    }

    template<class Key, class Predicate, class Allocator>
    void Scribe<std::multiset<Key, Predicate, Allocator>>::Scriven(const std::string& name, ObjectT& object, Archive::Json& archive)
    {
        if (archive.IsOutput())
        {
            auto outputArchive = archive.AsOutput();

            outputArchive->StartList(name);
            for (auto loop = object.begin(); loop != object.end(); ++loop)
                archive("", *loop);
            outputArchive->EndList();
        }
        else
        {
            object.clear();

            auto inputArchive = archive.AsInput();

            auto size = inputArchive->StartList(name);

            while (size-- > 0)
            {
                ScopeConstructor<typename ObjectT::value_type> constructor(archive);

                auto emplaced = object.emplace(std::move(constructor.GetMove()));
                if (object.count(*constructor.Get()) == 1)
                    archive.types.AttemptReplaceTrackedObject(*constructor.Get(), RemoveConst(*emplaced));
            }

            inputArchive->EndList();
        }
    }

    template<class Key, class Predicate, class Allocator, class Archive>
    struct ScribeTraits<std::multiset<Key, Predicate, Allocator>, Archive>
    {
        using Category = TrackingScribeCategory<std::multiset<Key, Predicate, Allocator>>;
    };
}