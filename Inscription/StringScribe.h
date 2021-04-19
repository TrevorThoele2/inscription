#pragma once

#include <string>

#include "TrackingScribeCategory.h"

#include "NumericScribe.h"

namespace Inscription
{
    template<>
    class Scribe<std::string> final
    {
    public:
        using ObjectT = std::string;
    public:
        void Scriven(ObjectT& object, Archive::Binary& archive);
        void Scriven(const std::string& name, ObjectT& object, Archive::Json& archive);
        void Scriven(ObjectT& object, Archive::Text& archive);
    };

    template<class Archive>
    struct ScribeTraits<std::string, Archive>
    {
        using Category = TrackingScribeCategory<std::string>;
    };
}