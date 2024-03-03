
#pragma once

#include <forward_list>
#include "Scribe.h"
#include "ContainerSize.h"
#include "StackConstructor.h"

namespace Inscription
{
    template<class T, class Alloc>
    void Save(Scribe &scribe, std::forward_list<T, Alloc> &obj)
    {
        ContainerSize size(std::distance(obj.begin(), obj.end()));
        scribe.Save(size);
        for (auto loop = obj.begin(); loop != obj.end(); ++loop)
            scribe.Save(*loop);
    }

    template<class T, class Alloc>
    void Load(Scribe &scribe, std::forward_list<T, Alloc> &obj)
    {
        typedef std::forward_list<T, Alloc> ContainerT;

        ContainerSize size;
        scribe.Load(size);

        obj.clear();
        while (size-- > 0)
        {
            StackConstructor<typename ContainerT::value_type> constructor(scribe);
            obj.push_front(std::move(constructor.GetMove()));
            scribe.ReplaceTrackedObject(*constructor.Get(), obj.front());
        }
    }

    template<class T, class Alloc>
    void Serialize(Scribe &scribe, std::forward_list<T, Alloc> &obj)
    {
        (scribe.IsOutput()) ? Save(scribe, obj) : Load(scribe, obj);
    }
}