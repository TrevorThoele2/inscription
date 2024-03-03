#pragma once

#include <array>

#include "ScopedConstructor.h"

namespace Inscription
{
    template<class ScribeT, class T, std::size_t N>
    void Save(ScribeT& scribe, std::array<T, N>& obj)
    {
        for (auto loop = obj.begin(); loop != obj.end(); ++loop)
            scribe.Save(*loop);
    }

    template<class ScribeT, class T, std::size_t N>
    void Load(ScribeT& scribe, std::array<T, N>& obj)
    {
        typedef std::array<T, N> ContainerT;

        std::size_t count = 0;
        while (count < N)
        {
            ScopedConstructor<typename ContainerT::value_type> constructor(scribe);
            obj[count] = std::move(constructor.GetMove());
            scribe.ReplaceTrackedObject(*constructor.Get(), obj.back());
            ++count;
        }
    }

    template<class ScribeT, class T, std::size_t N>
    void Serialize(ScribeT& scribe, std::array<T, N>& obj)
    {
        (scribe.IsOutput()) ? Save(*scribe.AsOutput(), obj) : Load(*scribe.AsInput(), obj);
    }
}