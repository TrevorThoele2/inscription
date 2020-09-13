#pragma once

#include <string>

#include "TrackingScribeCategory.h"

#include "NumericScribe.h"

namespace Inscription
{
    class BinaryArchive;
    class TextArchive;

    template<>
    class Scribe<std::string> final
    {
    public:
        using ObjectT = std::string;
    public:
        void Scriven(ObjectT& object, BinaryArchive& archive);
        void Scriven(const std::string& name, ObjectT& object, JsonArchive& archive);
        void Scriven(ObjectT& object, TextArchive& archive);
    };

    template<class Archive>
    struct ScribeTraits<std::string, Archive>
    {
        using Category = TrackingScribeCategory<std::string>;
    };
}