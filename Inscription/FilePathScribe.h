#pragma once

#include <filesystem>

#include "TrackingScribe.h"

namespace Inscription
{
    class BinaryArchive;
    class TextArchive;

    template<>
    class Scribe<std::filesystem::path, BinaryArchive> final :
        public TrackingScribe<std::filesystem::path, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };

    template<>
    class Scribe<std::filesystem::path, JsonArchive> final :
        public TrackingScribe<std::filesystem::path, JsonArchive>
    {
    protected:
        void ScrivenImplementation(const std::string& name, ObjectT& object, ArchiveT& archive) override;
    };

    template<>
    class Scribe<std::filesystem::path, TextArchive> final :
        public ScribeBase<std::filesystem::path, TextArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}