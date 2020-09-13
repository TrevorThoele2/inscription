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
        static void Scriven(ObjectT& object, BinaryArchive& archive);
        static void Scriven(const std::string& name, ObjectT& object, JsonArchive& archive);
        static void Scriven(ObjectT& object, TextArchive& archive);
    };

    template<class Object>
    void NullScribeCategory<Object>::Scriven(ObjectT& object, BinaryArchive& archive)
    {}

    template<class Object>
    void NullScribeCategory<Object>::Scriven(const std::string& name, ObjectT& object, JsonArchive& archive)
    {}

    template<class Object>
    void NullScribeCategory<Object>::Scriven(ObjectT& object, TextArchive& archive)
    {}
}