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
        void Scriven(ObjectT& object, Archive::Binary& archive);
        void Scriven(const std::string& name, ObjectT& object, Archive::Json& archive);
        void Scriven(ObjectT& object, Archive::Text& archive);
    };

    template<class Archive>
    struct ScribeTraits<std::filesystem::path, Archive>
    {
        using Category = TrackingScribeCategory<std::filesystem::path>;
    };
}