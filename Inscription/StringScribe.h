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
        void Scriven(ObjectT& object, Format::Binary& format);
        void Scriven(const std::string& name, ObjectT& object, Format::Json& format);
        void Scriven(ObjectT& object, Format::Plaintext& format);
    };

    template<class Format>
    struct ScribeTraits<std::string, Format>
    {
        using Category = TrackingScribeCategory<std::string>;
    };
}