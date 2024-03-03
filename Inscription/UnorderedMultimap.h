#pragma once

#include <unordered_map>

#include "ContainerSize.h"
#include "ScopedConstructor.h"
#include "Const.h"

namespace Inscription
{
    template<class ScribeT, class Key, class T, class Hash, class Pred, class Alloc>
    void Save(ScribeT& scribe, std::unordered_multimap<Key, T, Hash, Pred, Alloc>& obj)
    {
        ContainerSize size(obj.size());
        scribe.Save(size);
        for (auto loop = obj.begin(); loop != obj.end(); ++loop)
        {
            scribe.Save(RemoveConst(loop->first));
            scribe.Save(loop->second);
        }
    }

    template<class ScribeT, class Key, class T, class Hash, class Pred, class Alloc>
    void Load(ScribeT& scribe, std::unordered_multimap<Key, T, Hash, Pred, Alloc>& obj)
    {
        typedef std::unordered_multimap<Key, T, Hash, Pred, Alloc> ContainerT;

        ContainerSize size;
        scribe.Load(size);

        obj.clear();
        while (size-- > 0)
        {
            ScopedConstructor<typename ContainerT::key_type> key(scribe);
            ScopedConstructor<typename ContainerT::mapped_type> mapped(scribe);

            auto emplaced = obj.emplace(std::move(*key.Get()), std::move(mapped.GetMove()));
            if (obj.count(*key.Get()) == 1)
            {
                scribe.ReplaceTrackedObject(*key.Get(), RemoveConst(emplaced->first));
                scribe.ReplaceTrackedObject(*mapped.Get(), emplaced->second);
            }
        }
    }

    template<class ScribeT, class Key, class T, class Hash, class Pred, class Alloc>
    void Serialize(ScribeT& scribe, std::unordered_multimap<Key, T, Hash, Pred, Alloc>& obj)
    {
        (scribe.IsOutput()) ? Save(*scribe.AsOutput(), obj) : Load(*scribe.AsInput(), obj);
    }
}