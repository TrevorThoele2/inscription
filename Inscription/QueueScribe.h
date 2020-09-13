#pragma once

#include <queue>

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

    template<class T, class Container>
    class Scribe<std::queue<T, Container>> final
    {
    public:
        using ObjectT = std::queue<T, Container>;
    public:
        void Scriven(ObjectT& object, BinaryArchive& archive);
        void Scriven(const std::string& name, ObjectT& object, JsonArchive& archive);
    };

    template<class T, class Container>
    void Scribe<std::queue<T, Container>>::Scriven(ObjectT& object, BinaryArchive& archive)
    {
        if (archive.IsOutput())
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
        else
        {
            object = ObjectT();

            ContainerSize size;
            archive(size);

            while (size-- > 0)
            {
                ScopeConstructor<typename ObjectT::value_type> constructor(archive);
                object.push(std::move(constructor.GetMove()));
                archive.types.AttemptReplaceTrackedObject(*constructor.Get(), object.back());
            }
        }
    }

    template<class T, class Container>
    void Scribe<std::queue<T, Container>>::Scriven(const std::string& name, ObjectT& object, JsonArchive& archive)
    {
        if (archive.IsOutput())
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
        else
        {
            object = ObjectT();

            auto inputArchive = archive.AsInput();

            auto size = inputArchive->StartList(name);

            while (size-- > 0)
            {
                ScopeConstructor<typename ObjectT::value_type> constructor(archive);
                object.push(std::move(constructor.GetMove()));
                archive.types.AttemptReplaceTrackedObject(*constructor.Get(), object.back());
            }

            inputArchive->EndList();
        }
    }

    template<class T, class Container, class Archive>
    struct ScribeTraits<std::queue<T, Container>, Archive>
    {
        using Category = TrackingScribeCategory<std::queue<T, Container>>;
    };
}