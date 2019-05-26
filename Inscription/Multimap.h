#pragma once

#include <map>

#include "BinaryScribe.h"

#include "ContainerSize.h"
#include "ScopedConstructor.h"
#include "Const.h"

namespace Inscription
{
    template<class Key, class T, class Hash, class Alloc>
    void Save(OutputBinaryScribe& scribe, std::multimap<Key, T, Hash, Alloc>& obj)
    {
        ContainerSize size(obj.size());
        scribe.Save(size);
        for (auto loop = obj.begin(); loop != obj.end(); ++loop)
        {
            scribe.Save(RemoveConst(loop->first));
            scribe.Save(loop->second);
        }
    }

    template<class Key, class T, class Hash, class Alloc>
    void Load(InputBinaryScribe& scribe, std::multimap<Key, T, Hash, Alloc>& obj)
    {
        typedef std::multimap<Key, T, Hash, Alloc> ContainerT;

        ContainerSize size;
        scribe.Load(size);

        obj.clear();
        while (size-- > 0)
        {
            ScopedConstructor<typename ContainerT::key_type> key(scribe);
            ScopedConstructor<typename ContainerT::mapped_type> mapped(scribe);

            auto emplaced = obj.emplace(std::move(key.GetMove()), std::move(mapped.GetMove()));
            if (obj.count(*key.Get()) == 1)
            {
                scribe.ReplaceTrackedObject(*key.Get(), RemoveConst(emplaced->first));
                scribe.ReplaceTrackedObject(*mapped.Get(), emplaced->second);
            }
        }
    }

    template<class Key, class T, class Hash, class Alloc>
    void Serialize(BinaryScribe& scribe, std::multimap<Key, T, Hash, Alloc>& obj)
    {
        (scribe.IsOutput()) ? Save(*scribe.AsOutput(), obj) : Load(*scribe.AsInput(), obj);
    }
}