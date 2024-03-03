#pragma once

#include <list>

#include "TrackingScribeCategory.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"

#include "OutputBinaryFormat.h"
#include "InputBinaryFormat.h"
#include "OutputJsonFormat.h"
#include "InputJsonFormat.h"

namespace Inscription
{
    template<class T, class Allocator>
    class Scribe<std::list<T, Allocator>> final
    {
    public:
        using ObjectT = std::list<T, Allocator>;
    public:
        void Scriven(ObjectT& object, Format::Binary& format);
        void Scriven(const std::string& name, ObjectT& object, Format::Json& format);
    };

    template<class T, class Allocator>
    void Scribe<std::list<T, Allocator>>::Scriven(ObjectT& object, Format::Binary& format)
    {
        if (format.IsOutput())
        {
            ContainerSize size(std::distance(object.begin(), object.end()));
            format(size);
            for (auto loop = object.begin(); loop != object.end(); ++loop)
                format(*loop);
        }
        else
        {
            ContainerSize size;
            format(size);

            object.clear();
            while (size-- > 0)
            {
                ScopeConstructor<typename ObjectT::value_type> constructor(format);
                object.push_back(std::move(constructor.GetMove()));
                format.types.AttemptReplaceTrackedObject(*constructor.Get(), object.front());
            }
        }
    }

    template<class T, class Allocator>
    void Scribe<std::list<T, Allocator>>::Scriven(const std::string& name, ObjectT& object, Format::Json& format)
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
            object.clear();

            const auto inputFormat = format.AsInput();

            auto size = inputFormat->StartList(name);

            size_t i = 0;
            while (size-- > 0)
            {
                ScopeConstructor<typename ObjectT::value_type> constructor(Chroma::ToString(i), format);
                object.push_back(std::move(constructor.GetMove()));
                format.types.AttemptReplaceTrackedObject(*constructor.Get(), object.front());
                ++i;
            }

            inputFormat->EndList();
        }
    }

    template<class T, class Allocator, class Format>
    struct ScribeTraits<std::list<T, Allocator>, Format>
    {
        using Category = TrackingScribeCategory<std::list<T, Allocator>>;
    };
}