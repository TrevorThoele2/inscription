#pragma once

#include <filesystem>

#include "TrackingScribeCategory.h"

namespace Inscription
{
    template<>
    class Scribe<std::filesystem::path> final
    {
    public:
        using ObjectT = std::filesystem::path;
    public:
        void Scriven(ObjectT& object, Format::Binary& format);
        void Scriven(const std::string& name, ObjectT& object, Format::Json& format);
        void Scriven(ObjectT& object, Format::Plaintext& format);
    };

    template<class Format>
    struct ScribeTraits<std::filesystem::path, Format>
    {
        using Category = TrackingScribeCategory<std::filesystem::path>;
    };
}