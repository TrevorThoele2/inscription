#pragma once

#include <stack>

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
    class Scribe<std::stack<T, Container>> final
    {
    public:
        using ObjectT = std::stack<T, Container>;
    public:
        void Scriven(ObjectT& object, BinaryArchive& archive);
        void Scriven(const std::string& name, ObjectT& object, JsonArchive& archive);
    };

    template<class T, class Container>
    void Scribe<std::stack<T, Container>>::Scriven(ObjectT& object, BinaryArchive& archive)
    {
        if (archive.IsOutput())
        {
            auto copied = object;

            ObjectT reversed;
            while (!copied.empty())
            {
                auto& top = copied.top();
                reversed.push(top);
                copied.pop();
            }

            ContainerSize size(reversed.size());
            archive(size);
            while (!reversed.empty())
            {
                auto& top = reversed.top();
                archive(top);
                reversed.pop();
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
                archive.types.AttemptReplaceTrackedObject(*constructor.Get(), object.top());
            }
        }
    }

    template<class T, class Container>
    void Scribe<std::stack<T, Container>>::Scriven(const std::string& name, ObjectT& object, JsonArchive& archive)
    {
        if (archive.IsOutput())
        {
            auto copied = object;

            ObjectT reversed;
            while (!copied.empty())
            {
                auto& top = copied.top();
                reversed.push(top);
                copied.pop();
            }

            auto outputArchive = archive.AsOutput();

            outputArchive->StartList(name);
            while (!reversed.empty())
            {
                auto& top = reversed.top();
                archive("", top);
                reversed.pop();
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
                archive.types.AttemptReplaceTrackedObject(*constructor.Get(), object.top());
            }

            inputArchive->EndList();
        }
    }

    template<class T, class Container>
    struct ScribeTraits<std::stack<T, Container>, Archive>
    {
        using Category = TrackingScribeCategory<std::stack<T, Container>>;
    };
}