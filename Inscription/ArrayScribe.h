#pragma once

#include <array>

#include "TrackingScribeCategory.h"

#include "ScopeConstructor.h"

#include "OutputBinaryFormat.h"
#include "InputBinaryFormat.h"
#include "OutputJsonFormat.h"
#include "InputJsonFormat.h"

namespace Inscription
{
    template<class T, size_t N>
    class Scribe<std::array<T, N>> final
    {
    public:
        using ObjectT = std::array<T, N>;
    public:
        void Scriven(ObjectT& object, Format::Binary& format);
        void Scriven(const std::string& name, ObjectT& object, Format::Json& format);
    };

    template<class T, size_t N>
    void Scribe<std::array<T, N>>::Scriven(ObjectT& object, Format::Binary& format)
    {
        if (format.IsOutput())
        {
            for (auto value = object.begin(); value != object.end(); ++value)
                format(*value);
        }
        else
        {
            std::size_t count = 0;
            while (count < N)
            {
                ScopeConstructor<typename ObjectT::value_type> constructor(format);
                object[count] = std::move(constructor.GetMove());
                format.types.AttemptReplaceTrackedObject(*constructor.Get(), object.back());
                ++count;
            }
        }
    }

    template<class T, size_t N>
    void Scribe<std::array<T, N>>::Scriven(const std::string& name, ObjectT& object, Format::Json& format)
    {
        if (format.IsOutput())
        {
            const auto outputFormat = format.AsOutput();

            outputFormat->StartList(name);
            size_t i = 0;
            for (auto loop = object.begin(); loop != object.end(); ++loop, ++i)
                format(Chroma::ToString(i), *loop);
            outputFormat->EndList();
        }
        else
        {
            const auto inputFormat = format.AsInput();

            auto size = inputFormat->StartList(name);

            size_t i = 0;
            while (size-- > 0)
            {
                ScopeConstructor<typename ObjectT::value_type> constructor(Chroma::ToString(i), format);
                object[i] = std::move(constructor.GetMove());
                format.types.AttemptReplaceTrackedObject(*constructor.Get(), object.front());
                ++i;
            }

            inputFormat->EndList();
        }
    }

    template<class T, size_t N, class Format>
    struct ScribeTraits<std::array<T, N>, Format>
    {
        using Category = TrackingScribeCategory<std::array<T, N>>;
    };
}