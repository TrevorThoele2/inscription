#pragma once

#include <string>

#include "Composite.h"

#include "Numeric.h"

namespace Inscription
{
    class BinaryArchive;

    template<>
    class Scribe<std::string, BinaryArchive> final :
        public CompositeScribe<std::string, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    private:
        static void SaveImplementation(ObjectT& object, ArchiveT& archive);
        static void LoadImplementation(ObjectT& object, ArchiveT& archive);
    };
}