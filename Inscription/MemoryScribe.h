#pragma once

#include <memory>

#include "TrackingScribeCategory.h"
#include "PointerScribe.h"

namespace Inscription
{
    template<class T, class Deleter>
    class Scribe<std::unique_ptr<T, Deleter>> final
    {
    public:
        using ObjectT = std::unique_ptr<T, Deleter>;
    public:
        void Scriven(ObjectT& object, Format::Binary& format);
    };

    template<class T, class Deleter>
    void Scribe<std::unique_ptr<T, Deleter>>::Scriven(ObjectT& object, Format::Binary& format)
    {
        if (format.IsOutput())
        {
            T* saved = object.get();
            format(saved);
        }
        else
        {
            T* loaded = nullptr;
            format(loaded);
            object.reset(loaded);
        }
    }

    template<class T, class Deleter, class Format>
    struct ScribeTraits<std::unique_ptr<T, Deleter>, Format>
    {
        using Category = TrackingScribeCategory<std::unique_ptr<T, Deleter>>;
    };
}