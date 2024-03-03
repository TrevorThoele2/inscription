#pragma once

#include "BinaryFormat.h"
#include "JsonFormat.h"
#include "PlaintextFormat.h"

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
        static void Scriven(ObjectT& object, Format::Binary& format);
        static void Scriven(const std::string& name, ObjectT& object, Format::Json& format);
        static void Scriven(ObjectT& object, Format::Plaintext& format);
    };

    template<class Object>
    void NullScribeCategory<Object>::Scriven(ObjectT& object, Format::Binary& format)
    {}

    template<class Object>
    void NullScribeCategory<Object>::Scriven(const std::string& name, ObjectT& object, Format::Json& format)
    {}

    template<class Object>
    void NullScribeCategory<Object>::Scriven(ObjectT& object, Format::Plaintext& format)
    {}
}