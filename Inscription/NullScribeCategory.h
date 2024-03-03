#pragma once

#include "BinaryArchive.h"
#include "JsonArchive.h"
#include "TextArchive.h"

namespace Inscription
{
    template<class Object>
    class NullScribeCategory final
    {
    public:
        using ObjectT = Object;
    public:
        static constexpr bool requiresScribe = false;
    public:
        static void Scriven(ObjectT& object, Archive::Binary& archive);
        static void Scriven(const std::string& name, ObjectT& object, Archive::Json& archive);
        static void Scriven(ObjectT& object, Archive::Text& archive);
    };

    template<class Object>
    void NullScribeCategory<Object>::Scriven(ObjectT& object, Archive::Binary& archive)
    {}

    template<class Object>
    void NullScribeCategory<Object>::Scriven(const std::string& name, ObjectT& object, Archive::Json& archive)
    {}

    template<class Object>
    void NullScribeCategory<Object>::Scriven(ObjectT& object, Archive::Text& archive)
    {}
}