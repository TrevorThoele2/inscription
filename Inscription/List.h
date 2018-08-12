
#pragma once

#include <list>
#include "Scribe.h"
#include "ContainerSize.h"
#include "StackConstructor.h"

namespace Inscription
{
    template<class T, class Alloc>
    void Save(Scribe &scribe, std::list<T, Alloc> &obj)
    {
        ContainerSize size(obj.size());
        scribe.Save(size);
        for (auto loop = obj.begin(); loop != obj.end(); ++loop)
            scribe.Save(*loop);
    }

    template<class T, class Alloc>
    void Load(Scribe &scribe, std::list<T, Alloc> &obj)
    {
        typedef std::list<T, Alloc> ContainerT;

        ContainerSize size;
        scribe.Load(size);

        obj.clear();
        while (size-- > 0)
        {
            StackConstructor<typename ContainerT::value_type> constructor(scribe);
            obj.push_back(std::move(constructor.GetMove()));
            scribe.ReplaceTrackedObject(*constructor.Get(), obj.back());
        }
    }

    template<class T, class Alloc>
    void Serialize(Scribe &scribe, std::list<T, Alloc> &obj)
    {
        (scribe.IsOutput()) ? Save(scribe, obj) : Load(scribe, obj);
    }
}