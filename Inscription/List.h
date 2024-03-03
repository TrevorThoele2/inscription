#pragma once

#include <list>

#include "ContainerSize.h"
#include "ScopedConstructor.h"

namespace Inscription
{
    template<class ScribeT, class T, class Alloc>
    void Save(ScribeT& scribe, std::list<T, Alloc>& obj)
    {
        ContainerSize size(obj.size());
        scribe.Save(size);
        for (auto loop = obj.begin(); loop != obj.end(); ++loop)
            scribe.Save(*loop);
    }

    template<class ScribeT, class T, class Alloc>
    void Load(ScribeT& scribe, std::list<T, Alloc>& obj)
    {
        typedef std::list<T, Alloc> ContainerT;

        ContainerSize size;
        scribe.Load(size);

        obj.clear();
        while (size-- > 0)
        {
            ScopedConstructor<typename ContainerT::value_type> constructor(scribe);
            obj.push_back(std::move(constructor.GetMove()));
            scribe.ReplaceTrackedObject(*constructor.Get(), obj.back());
        }
    }

    template<class ScribeT, class T, class Alloc>
    void Serialize(ScribeT& scribe, std::list<T, Alloc>& obj)
    {
        (scribe.IsOutput()) ? Save(*scribe.AsOutput(), obj) : Load(*scribe.AsInput(), obj);
    }
}