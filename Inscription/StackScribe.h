#pragma once

#include <stack>

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
    class Scribe<std::stack<T, Container>> final
    {
    public:
        using ObjectT = std::stack<T, Container>;
    public:
        void Scriven(ObjectT& object, Format::Binary& format);
        void Scriven(const std::string& name, ObjectT& object, Format::Json& format);
    };

    template<class T, class Container>
    void Scribe<std::stack<T, Container>>::Scriven(ObjectT& object, Format::Binary& format)
    {
        if (format.IsOutput())
        {
            auto copied = object;

            ObjectT reversed;
            while (!copied.empty())
            {
                auto& top = copied.top();
                reversed.push(top);
                copied.pop();
            }

            ContainerSize size(reversed.size());
            format(size);
            while (!reversed.empty())
            {
                auto& top = reversed.top();
                format(top);
                reversed.pop();
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
                format.types.AttemptReplaceTrackedObject(*constructor.Get(), object.top());
            }
        }
    }

    template<class T, class Container>
    void Scribe<std::stack<T, Container>>::Scriven(const std::string& name, ObjectT& object, Format::Json& format)
    {
        if (format.IsOutput())
        {
            auto copied = object;

            ObjectT reversed;
            while (!copied.empty())
            {
                auto& top = copied.top();
                reversed.push(top);
                copied.pop();
            }

            const auto outputFormat = format.AsOutput();

            outputFormat->StartList(name);
            size_t i = 0;
            while (!reversed.empty())
            {
                auto& top = reversed.top();
                format(Chroma::ToString(i), top);
                reversed.pop();
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
                format.types.AttemptReplaceTrackedObject(*constructor.Get(), object.top());
                ++i;
            }

            inputFormat->EndList();
        }
    }

    template<class T, class Container>
    struct ScribeTraits<std::stack<T, Container>, Format::Format>
    {
        using Category = TrackingScribeCategory<std::stack<T, Container>>;
    };
}