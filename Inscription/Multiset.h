#pragma once

#include <set>

#include "BinaryScribe.h"

#include "ContainerSize.h"
#include "ScopedConstructor.h"
#include "Const.h"

namespace Inscription
{
    template<class Key, class Pred, class Alloc>
    void Save(BinaryScribe& scribe, std::multiset<Key, Pred, Alloc>& obj)
    {
        ContainerSize size(obj.size());
        scribe.Save(size);
        for (auto loop = obj.begin(); loop != obj.end(); ++loop)
            scribe.Save(RemoveConst(*loop));
    }

    template<class Key, class Pred, class Alloc>
    void Load(BinaryScribe& scribe, std::multiset<Key, Pred, Alloc>& obj)
    {
        typedef std::multiset<Key, Pred, Alloc> ContainerT;

        ContainerSize size;
        scribe.Load(size);

        obj.clear();
        while (size-- > 0)
        {
            ScopedConstructor<typename ContainerT::value_type> constructor(scribe);

            auto emplaced = obj.emplace(std::move(constructor.GetMove()));
            if (obj.count(*constructor.Get()) == 1)
                scribe.ReplaceTrackedObject(*constructor.Get(), RemoveConst(*emplaced));
        }
    }

    template<class Key, class Pred, class Alloc>
    void Serialize(BinaryScribe& scribe, std::multiset<Key, Pred, Alloc>& obj)
    {
        (scribe.IsOutput()) ? Save(scribe, obj) : Load(scribe, obj);
    }
}