#pragma once

#include <stack>

#include "ContainerSize.h"
#include "ScopedConstructor.h"

namespace Inscription
{
    template<class ScribeT, class T, class Container>
    void Save(ScribeT& scribe, std::stack<T, Container>& obj)
    {
        auto copied = obj;

        std::stack<T, Container> reversed;
        while (!copied.empty())
        {
            auto& top = copied.top();
            reversed.push(top);
            copied.pop();
        }

        ContainerSize size(reversed.size());
        scribe.Save(size);
        while (!reversed.empty())
        {
            auto& top = reversed.top();
            scribe.Save(top);
            reversed.pop();
        }
    }

    template<class ScribeT, class T, class Container>
    void Load(ScribeT& scribe, std::stack<T, Container>& obj)
    {
        typedef std::stack<T, Container> ContainerT;

        ContainerSize size;
        scribe.Load(size);

        while (size-- > 0)
        {
            ScopedConstructor<typename ContainerT::value_type> constructor(scribe);
            obj.push(std::move(constructor.GetMove()));
            scribe.ReplaceTrackedObject(*constructor.Get(), obj.top());
        }
    }

    template<class ScribeT, class T, class Container>
    void Serialize(ScribeT& scribe, std::stack<T, Container>& obj)
    {
        (scribe.IsOutput()) ? Save(*scribe.AsOutput(), obj) : Load(*scribe.AsInput(), obj);
    }
}