
#pragma once

#include <unordered_set>
#include "Scribe.h"
#include "ContainerSize.h"
#include "StackConstructor.h"
#include "Const.h"

namespace Inscription
{
    template<class Key, class Hash, class Pred, class Alloc>
    void Save(Scribe &scribe, std::unordered_multiset<Key, Hash, Pred, Alloc> &obj)
    {
        ContainerSize size(obj.size());
        scribe.Save(size);
        for (auto loop = obj.begin(); loop != obj.end(); ++loop)
            scribe.Save(RemoveConst(*loop));
    }

    template<class Key, class Hash, class Pred, class Alloc>
    void Load(Scribe &scribe, std::unordered_multiset<Key, Hash, Pred, Alloc> &obj)
    {
        typedef std::unordered_multiset<Key, Hash, Pred, Alloc> ContainerT;

        ContainerSize size;
        scribe.Load(size);

        obj.clear();
        while (size-- > 0)
        {
            StackConstructor<typename ContainerT::value_type> constructor(scribe);

            auto emplaced = obj.emplace(std::move(constructor.GetMove()));
            if (obj.count(*constructor.Get()) == 1)
                scribe.ReplaceTrackedObject(*constructor.Get(), RemoveConst(*emplaced));
        }
    }

    template<class Key, class Hash, class Pred, class Alloc>
    void Serialize(Scribe &scribe, std::unordered_multiset<Key, Hash, Pred, Alloc> &obj)
    {
        (scribe.IsOutput()) ? Save(scribe, obj) : Load(scribe, obj);
    }
}