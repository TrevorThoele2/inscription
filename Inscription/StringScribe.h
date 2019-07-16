#pragma once

#include <string>

#include "CompositeScribe.h"

#include "NumericScribe.h"

namespace Inscription
{
    class BinaryArchive;
    class TextArchive;

    template<>
    class Scribe<std::string, BinaryArchive> final :
        public CompositeScribe<std::string, BinaryArchive>
    {
    public:
        static void ScrivenImplementation(ObjectT& object, ArchiveT& archive);
    private:
        static void SaveImplementation(ObjectT& object, ArchiveT& archive);
        static void LoadImplementation(ObjectT& object, ArchiveT& archive);
    };

    template<>
    class Scribe<std::string, TextArchive> final :
        public CompositeScribe<std::string, TextArchive>
    {
    public:
        static void ScrivenImplementation(ObjectT& object, ArchiveT& archive);
    private:
        static void SaveImplementation(ObjectT& object, ArchiveT& archive);
        static void LoadImplementation(ObjectT& object, ArchiveT& archive);
    };
}