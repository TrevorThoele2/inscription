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
    class BinaryArchive;

    template<class Key, class Predicate, class Allocator>
    class Scribe<std::set<Key, Predicate, Allocator>> final
    {
    public:
        using ObjectT = std::set<Key, Predicate, Allocator>;
    public:
        void Scriven(ObjectT& object, BinaryArchive& archive);
        void Scriven(const std::string& name, ObjectT& object, JsonArchive& archive);
    };

    template<class Key, class Predicate, class Allocator>
    void Scribe<std::set<Key, Predicate, Allocator>>::Scriven(ObjectT& object, BinaryArchive& archive)
    {
        if (archive.IsOutput())
        {
            ContainerSize size(object.size());
            archive(size);
            for (auto loop = object.begin(); loop != object.end(); ++loop)
                archive(*loop);
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
                if (emplaced.second)
                    archive.types.AttemptReplaceTrackedObject(*constructor.Get(), RemoveConst(*emplaced.first));
            }
        }
    }

    template<class Key, class Predicate, class Allocator>
    void Scribe<std::set<Key, Predicate, Allocator>>::Scriven(const std::string& name, ObjectT& object, JsonArchive& archive)
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
                if (emplaced.second)
                    archive.types.AttemptReplaceTrackedObject(*constructor.Get(), RemoveConst(*emplaced.first));
            }

            inputArchive->EndList();
        }
    }

    template<class Key, class Predicate, class Allocator, class Archive>
    struct ScribeTraits<std::set<Key, Predicate, Allocator>, Archive>
    {
        using Category = TrackingScribeCategory<std::set<Key, Predicate, Allocator>>;
    };
}