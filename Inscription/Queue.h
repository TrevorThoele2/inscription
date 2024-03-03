#pragma once

#include <queue>

#include "ContainerSize.h"
#include "ScopedConstructor.h"

namespace Inscription
{
    template<class ScribeT, class T, class Container>
    void Save(ScribeT& scribe, std::queue<T, Container>& obj)
    {
        auto copied = obj;

        ContainerSize size(copied.size());
        scribe.Save(size);
        while (!copied.empty())
        {
            auto& front = copied.front();
            scribe.Save(front);
            copied.pop();
        }
    }

    template<class ScribeT, class T, class Container>
    void Load(ScribeT& scribe, std::queue<T, Container>& obj)
    {
        typedef std::queue<T, Container> ContainerT;

        ContainerSize size;
        scribe.Load(size);

        while (size-- > 0)
        {
            ScopedConstructor<typename ContainerT::value_type> constructor(scribe);
            obj.push(std::move(constructor.GetMove()));
            scribe.ReplaceTrackedObject(*constructor.Get(), obj.back());
        }
    }

    template<class ScribeT, class T, class Container>
    void Serialize(ScribeT& scribe, std::queue<T, Container>& obj)
    {
        (scribe.IsOutput()) ? Save(*scribe.AsOutput(), obj) : Load(*scribe.AsInput(), obj);
    }
}