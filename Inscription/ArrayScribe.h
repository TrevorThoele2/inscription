#pragma once

#include <array>

#include "TrackingScribeCategory.h"

#include "ScopeConstructor.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"
#include "OutputJsonArchive.h"
#include "InputJsonArchive.h"

namespace Inscription
{
    template<class T, size_t N>
    class Scribe<std::array<T, N>> final
    {
    public:
        using ObjectT = std::array<T, N>;
    public:
        void Scriven(ObjectT& object, Archive::Binary& archive);
        void Scriven(const std::string& name, ObjectT& object, Archive::Json& archive);
    };

    template<class T, size_t N>
    void Scribe<std::array<T, N>>::Scriven(ObjectT& object, Archive::Binary& archive)
    {
        if (archive.IsOutput())
        {
            for (auto value = object.begin(); value != object.end(); ++value)
                archive(*value);
        }
        else
        {
            std::size_t count = 0;
            while (count < N)
            {
                ScopeConstructor<typename ObjectT::value_type> constructor(archive);
                object[count] = std::move(constructor.GetMove());
                archive.types.AttemptReplaceTrackedObject(*constructor.Get(), object.back());
                ++count;
            }
        }
    }

    template<class T, size_t N>
    void Scribe<std::array<T, N>>::Scriven(const std::string& name, ObjectT& object, Archive::Json& archive)
    {
        if (archive.IsOutput())
        {
            auto outputArchive = archive.AsOutput();

            outputArchive->StartList(name);
            for (auto loop = object.begin(); loop != object.end(); ++loop)
                archive("", *loop);
            outputArchive->EndList();
        }
        else
        {
            auto inputArchive = archive.AsInput();

            auto size = inputArchive->StartList(name);

            auto i = 0;
            while (size-- > 0)
            {
                ScopeConstructor<typename ObjectT::value_type> constructor(archive);
                object[i] = std::move(constructor.GetMove());
                archive.types.AttemptReplaceTrackedObject(*constructor.Get(), object.front());
                ++i;
            }

            inputArchive->EndList();
        }
    }

    template<class T, size_t N, class Archive>
    struct ScribeTraits<std::array<T, N>, Archive>
    {
        using Category = TrackingScribeCategory<std::array<T, N>>;
    };
}