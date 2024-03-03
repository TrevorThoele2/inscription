#pragma once

#include <filesystem>

#include "TrackingScribeCategory.h"

namespace Inscription
{
    class BinaryArchive;
    class TextArchive;

    template<>
    class Scribe<std::filesystem::path> final
    {
    public:
        using ObjectT = std::filesystem::path;
    public:
        void Scriven(ObjectT& object, BinaryArchive& archive);
        void Scriven(const std::string& name, ObjectT& object, JsonArchive& archive);
        void Scriven(ObjectT& object, TextArchive& archive);
    };

    template<class Archive>
    struct ScribeTraits<std::filesystem::path, Archive>
    {
        using Category = TrackingScribeCategory<std::filesystem::path>;
    };
}