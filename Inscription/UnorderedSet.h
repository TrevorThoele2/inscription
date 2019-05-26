#pragma once

#include <unordered_set>

#include "BinaryScribe.h"

#include "ContainerSize.h"
#include "ScopedConstructor.h"
#include "Const.h"

namespace Inscription
{
    template<class Key, class Hash, class Pred, class Alloc>
    void Save(OutputBinaryScribe& scribe, std::unordered_set<Key, Hash, Pred, Alloc>& obj)
    {
        ContainerSize size(obj.size());
        scribe.Save(size);
        for (auto loop = obj.begin(); loop != obj.end(); ++loop)
            scribe.Save(RemoveConst(*loop));
    }

    template<class Key, class Hash, class Pred, class Alloc>
    void Load(InputBinaryScribe& scribe, std::unordered_set<Key, Hash, Pred, Alloc>& obj)
    {
        typedef std::unordered_set<Key, Hash, Pred, Alloc> ContainerT;

        ContainerSize size;
        scribe.Load(size);

        obj.clear();
        while (size-- > 0)
        {
            ScopedConstructor<typename ContainerT::value_type> constructor(scribe);

            auto emplaced = obj.emplace(std::move(constructor.GetMove()));
            if (emplaced.second)
                scribe.ReplaceTrackedObject(*constructor.Get(), RemoveConst(*emplaced.first));
        }
    }

    template<class Key, class Hash, class Pred, class Alloc>
    void Serialize(BinaryScribe& scribe, std::unordered_set<Key, Hash, Pred, Alloc>& obj)
    {
        (scribe.IsOutput()) ? Save(*scribe.AsOutput(), obj) : Load(*scribe.AsInput(), obj);
    }
}