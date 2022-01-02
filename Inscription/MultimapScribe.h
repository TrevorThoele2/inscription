#pragma once

#include <map>

#include "TrackingScribeCategory.h"

#include "ContainerSize.h"
#include "ScopeConstructor.h"

#include "OutputBinaryFormat.h"
#include "InputBinaryFormat.h"
#include "OutputJsonFormat.h"
#include "InputJsonFormat.h"

namespace Inscription
{
    template<class Key, class T, class Hash, class Allocator>
    class Scribe<std::multimap<Key, T, Hash, Allocator>> final
    {
    public:
        using ObjectT = std::multimap<Key, T, Hash, Allocator>;
    public:
        void Scriven(ObjectT& object, Format::Binary& format);
        void Scriven(const std::string& name, ObjectT& object, Format::Json& format);
    };

    template<class Key, class T, class Hash, class Allocator>
    void Scribe<std::multimap<Key, T, Hash, Allocator>>::Scriven(ObjectT& object, Format::Binary& format)
    {
        if (format.IsOutput())
        {
            ContainerSize size(object.size());
            format(size);
            for (auto loop = object.begin(); loop != object.end(); ++loop)
            {
                format(loop->first);
                format(loop->second);
            }
        }
        else
        {
            ContainerSize size;
            format(size);

            object.clear();
            while (size-- > 0)
            {
                ScopeConstructor<typename ObjectT::key_type> key(format);
                ScopeConstructor<typename ObjectT::mapped_type> mapped(format);

                auto emplaced = object.emplace(std::move(key.GetMove()), std::move(mapped.GetMove()));
                if (object.count(*key.Get()) == 1)
                {
                    format.types.AttemptReplaceTrackedObject(*key.Get(), RemoveConst(emplaced->first));
                    format.types.AttemptReplaceTrackedObject(*mapped.Get(), emplaced->second);
                }
            }
        }
    }

    template<class Key, class T, class Hash, class Allocator>
    void Scribe<std::multimap<Key, T, Hash, Allocator>>::Scriven(const std::string& name, ObjectT& object, Format::Json& format)
    {
        if (format.IsOutput())
        {
            const auto outputFormat = format.AsOutput();

            outputFormat->StartList(name);
            for (auto loop = object.begin(); loop != object.end(); ++loop)
            {
                outputFormat->StartList("");
                format("0", loop->first);
                format("1", loop->second);
                outputFormat->EndList();
            }
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
                inputFormat->StartList(Chroma::ToString(i));

                ScopeConstructor<typename ObjectT::key_type> key("0", format);
                ScopeConstructor<typename ObjectT::mapped_type> mapped("1", format);

                auto emplaced = object.emplace(std::move(key.GetMove()), std::move(mapped.GetMove()));
                if (object.count(*key.Get()) == 1)
                {
                    format.types.AttemptReplaceTrackedObject(*key.Get(), RemoveConst(emplaced->first));
                    format.types.AttemptReplaceTrackedObject(*mapped.Get(), emplaced->second);
                }

                inputFormat->EndList();
                ++i;
            }

            inputFormat->EndList();
        }
    }

    template<class Key, class T, class Hash, class Allocator, class Format>
    struct ScribeTraits<std::multimap<Key, T, Hash, Allocator>, Format>
    {
        using Category = TrackingScribeCategory<std::multimap<Key, T, Hash, Allocator>>;
    };
}