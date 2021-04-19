#pragma once

#include <forward_list>

#include "TrackingScribeCategory.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"
#include "OutputJsonArchive.h"
#include "InputJsonArchive.h"

namespace Inscription
{
    template<class T, class Allocator>
    class Scribe<std::forward_list<T, Allocator>> final
    {
    public:
        using ObjectT = std::forward_list<T, Allocator>;
    public:
        void Scriven(ObjectT& object, Archive::Binary& archive);
        void Scriven(const std::string& name, ObjectT& object, Archive::Json& archive);
    };

    template<class T, class Allocator>
    void Scribe<std::forward_list<T, Allocator>>::Scriven(ObjectT& object, Archive::Binary& archive)
    {
        if (archive.IsOutput())
        {
            ContainerSize size(std::distance(object.begin(), object.end()));
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
                object.push_front(std::move(constructor.GetMove()));
                archive.types.AttemptReplaceTrackedObject(*constructor.Get(), object.front());
            }

            object.reverse();
        }
    }

    template<class T, class Allocator>
    void Scribe<std::forward_list<T, Allocator>>::Scriven(const std::string& name, ObjectT& object, Archive::Json& archive)
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
                object.push_front(std::move(constructor.GetMove()));
                archive.types.AttemptReplaceTrackedObject(*constructor.Get(), object.front());
            }

            inputArchive->EndList();

            object.reverse();
        }
    }

    template<class T, class Allocator, class Archive>
    struct ScribeTraits<std::forward_list<T, Allocator>, Archive>
    {
        using Category = TrackingScribeCategory<std::forward_list<T, Allocator>>;
    };
}