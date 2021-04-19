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
        void Scriven(ObjectT& object, Archive::Binary& archive);
    };

    template<class T, class Deleter>
    void Scribe<std::unique_ptr<T, Deleter>>::Scriven(ObjectT& object, Archive::Binary& archive)
    {
        if (archive.IsOutput())
        {
            T* saved = object.get();
            archive(saved);
        }
        else
        {
            T* loaded = nullptr;
            archive(loaded);
            object.reset(loaded);
        }
    }

    template<class T, class Deleter, class Archive>
    struct ScribeTraits<std::unique_ptr<T, Deleter>, Archive>
    {
        using Category = TrackingScribeCategory<std::unique_ptr<T, Deleter>>;
    };
}