#pragma once

#include <set>

#include "TrackingScribeCategory.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"

#include "OutputBinaryFormat.h"
#include "InputBinaryFormat.h"
#include "OutputJsonFormat.h"
#include "InputJsonFormat.h"

namespace Inscription
{
    template<class Key, class Predicate, class Allocator>
    class Scribe<std::set<Key, Predicate, Allocator>> final
    {
    public:
        using ObjectT = std::set<Key, Predicate, Allocator>;
    public:
        void Scriven(ObjectT& object, Format::Binary& format);
        void Scriven(const std::string& name, ObjectT& object, Format::Json& format);
    };

    template<class Key, class Predicate, class Allocator>
    void Scribe<std::set<Key, Predicate, Allocator>>::Scriven(ObjectT& object, Format::Binary& format)
    {
        if (format.IsOutput())
        {
            ContainerSize size(object.size());
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

                auto emplaced = object.emplace(std::move(constructor.GetMove()));
                if (emplaced.second)
                    format.types.AttemptReplaceTrackedObject(*constructor.Get(), RemoveConst(*emplaced.first));
            }
        }
    }

    template<class Key, class Predicate, class Allocator>
    void Scribe<std::set<Key, Predicate, Allocator>>::Scriven(const std::string& name, ObjectT& object, Format::Json& format)
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

                auto emplaced = object.emplace(std::move(constructor.GetMove()));
                if (emplaced.second)
                    format.types.AttemptReplaceTrackedObject(*constructor.Get(), RemoveConst(*emplaced.first));
                ++i;
            }

            inputFormat->EndList();
        }
    }

    template<class Key, class Predicate, class Allocator, class Format>
    struct ScribeTraits<std::set<Key, Predicate, Allocator>, Format>
    {
        using Category = TrackingScribeCategory<std::set<Key, Predicate, Allocator>>;
    };
}