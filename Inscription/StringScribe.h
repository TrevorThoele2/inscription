#pragma once

#include <string>

#include "TrackingScribe.h"

#include "NumericScribe.h"

namespace Inscription
{
    class BinaryArchive;
    class TextArchive;

    template<>
    class Scribe<std::string, BinaryArchive> final :
        public TrackingScribe<std::string, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    private:
        void SaveImplementation(ObjectT& object, ArchiveT& archive);
        void LoadImplementation(ObjectT& object, ArchiveT& archive);
    };

    template<>
    class Scribe<std::string, TextArchive> final :
        public ScribeBase<std::string, TextArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    private:
        void SaveImplementation(ObjectT& object, ArchiveT& archive);
        void LoadImplementation(ObjectT& object, ArchiveT& archive);
    };
}