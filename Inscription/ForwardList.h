#pragma once

#include <forward_list>

#include "ContainerSize.h"
#include "ScopedConstructor.h"

namespace Inscription
{
    template<class ScribeT, class T, class Alloc>
    void Save(ScribeT& scribe, std::forward_list<T, Alloc>& obj)
    {
        ContainerSize size(std::distance(obj.begin(), obj.end()));
        scribe.Save(size);
        for (auto loop = obj.begin(); loop != obj.end(); ++loop)
            scribe.Save(*loop);
    }

    template<class ScribeT, class T, class Alloc>
    void Load(ScribeT& scribe, std::forward_list<T, Alloc>& obj)
    {
        typedef std::forward_list<T, Alloc> ContainerT;

        ContainerSize size;
        scribe.Load(size);

        obj.clear();
        while (size-- > 0)
        {
            ScopedConstructor<typename ContainerT::value_type> constructor(scribe);
            obj.push_front(std::move(constructor.GetMove()));
            scribe.ReplaceTrackedObject(*constructor.Get(), obj.front());
        }

        obj.reverse();
    }

    template<class ScribeT, class T, class Alloc>
    void Serialize(ScribeT& scribe, std::forward_list<T, Alloc>& obj)
    {
        (scribe.IsOutput()) ? Save(*scribe.AsOutput(), obj) : Load(*scribe.AsInput(), obj);
    }
}