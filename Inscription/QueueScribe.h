#pragma once

#include <queue>

#include "TrackingScribeCategory.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"

#include "OutputBinaryFormat.h"
#include "InputBinaryFormat.h"
#include "OutputJsonFormat.h"
#include "InputJsonFormat.h"

namespace Inscription
{
    template<class T, class Container>
    class Scribe<std::queue<T, Container>> final
    {
    public:
        using ObjectT = std::queue<T, Container>;
    public:
        void Scriven(ObjectT& object, Format::Binary& format);
        void Scriven(const std::string& name, ObjectT& object, Format::Json& format);
    };

    template<class T, class Container>
    void Scribe<std::queue<T, Container>>::Scriven(ObjectT& object, Format::Binary& format)
    {
        if (format.IsOutput())
        {
            auto copied = object;

            ContainerSize size(copied.size());
            format(size);
            while (!copied.empty())
            {
                auto& front = copied.front();
                format(front);
                copied.pop();
            }
        }
        else
        {
            object = ObjectT();

            ContainerSize size;
            format(size);

            while (size-- > 0)
            {
                ScopeConstructor<typename ObjectT::value_type> constructor(format);
                object.push(std::move(constructor.GetMove()));
                format.types.AttemptReplaceTrackedObject(*constructor.Get(), object.back());
            }
        }
    }

    template<class T, class Container>
    void Scribe<std::queue<T, Container>>::Scriven(const std::string& name, ObjectT& object, Format::Json& format)
    {
        if (format.IsOutput())
        {
            const auto outputFormat = format.AsOutput();

            auto copied = object;

            outputFormat->StartList(name);
            size_t i = 0;
            while (!copied.empty())
            {
                auto& front = copied.front();
                format(Chroma::ToString(i), front);
                copied.pop();
                ++i;
            }
            outputFormat->EndList();
        }
        else
        {
            object = ObjectT();

            const auto inputFormat = format.AsInput();

            auto size = inputFormat->StartList(name);

            size_t i = 0;
            while (size-- > 0)
            {
                ScopeConstructor<typename ObjectT::value_type> constructor(Chroma::ToString(i), format);
                object.push(std::move(constructor.GetMove()));
                format.types.AttemptReplaceTrackedObject(*constructor.Get(), object.back());
                ++i;
            }

            inputFormat->EndList();
        }
    }

    template<class T, class Container, class Format>
    struct ScribeTraits<std::queue<T, Container>, Format>
    {
        using Category = TrackingScribeCategory<std::queue<T, Container>>;
    };
}