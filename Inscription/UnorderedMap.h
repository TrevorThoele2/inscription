
#pragma once

#include <unordered_map>
#include "Scribe.h"
#include "ContainerSize.h"
#include "StackConstructor.h"
#include "Const.h"

namespace inscription
{
    template<class Key, class T, class Hash, class Pred, class Alloc>
    void Save(Scribe &scribe, std::unordered_map<Key, T, Hash, Pred, Alloc> &obj)
    {
        ContainerSize size(obj.size());
        scribe.Save(size);
        for (auto loop = obj.begin(); loop != obj.end(); ++loop)
        {
            scribe.Save(RemoveConst(loop->first));
            scribe.Save(loop->second);
        }
    }

    template<class Key, class T, class Hash, class Pred, class Alloc>
    void Load(Scribe &scribe, std::unordered_map<Key, T, Hash, Pred, Alloc> &obj)
    {
        typedef std::unordered_map<Key, T, Hash, Pred, Alloc> ContainerT;

        ContainerSize size;
        scribe.Load(size);

        obj.clear();
        while (size-- > 0)
        {
            StackConstructor<typename ContainerT::key_type> key(scribe);
            StackConstructor<typename ContainerT::mapped_type> mapped(scribe);

            auto emplaced = obj.emplace(std::move(key.GetMove()), std::move(mapped.GetMove()));
            if (emplaced.second)
            {
                scribe.ReplaceTrackedObject(*key.Get(), RemoveConst(emplaced.first->first));
                scribe.ReplaceTrackedObject(*mapped.Get(), emplaced.first->second);
            }
        }
    }

    template<class Key, class T, class Hash, class Pred, class Alloc>
    void Serialize(Scribe &scribe, std::unordered_map<Key, T, Hash, Pred, Alloc> &obj)
    {
        (scribe.IsOutput()) ? Save(scribe, obj) : Load(scribe, obj);
    }
}