#pragma once

#include <array>
#include "Scribe.h"
#include "StackConstructor.h"

namespace inscription
{
    template<class T, std::size_t N>
    void Save(Scribe &scribe, std::array<T, N> &obj)
    {
        for (auto loop = obj.begin(); loop != obj.end(); ++loop)
            scribe.Save(*loop);
    }

    template<class T, std::size_t N>
    void Load(Scribe &scribe, std::array<T, N> &obj)
    {
        typedef std::array<T, N> ContainerT;

        std::size_t count = 0;
        while (count < N)
        {
            StackConstructor<typename ContainerT::value_type> constructor(scribe);
            obj[count] = std::move(constructor.GetMove());
            scribe.ReplaceTrackedObject(*constructor.Get(), obj.back());
            ++count;
        }
    }

    template<class T, std::size_t N>
    void Serialize(Scribe &scribe, std::array<T, N> &obj)
    {
        (scribe.IsOutput()) ? Save(scribe, obj) : Load(scribe, obj);
    }
}