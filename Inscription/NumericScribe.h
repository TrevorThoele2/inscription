#pragma once

#include <memory>

#include "Scribe.h"
#include "ScribeBase.h"

namespace Inscription
{
    template<class Object, class Archive>
    class NumericScribe : public ScribeBase<Object, Archive>
    {
    private:
        using BaseT = ScribeBase<Object, Archive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    public:
        using BaseT::Construct;
    private:
        constexpr static bool isEnum = std::is_enum_v<ObjectT>;
    private:
        NumericScribe() = delete;
        NumericScribe(const NumericScribe& arg) = delete;
        NumericScribe& operator=(const NumericScribe& arg) = delete;
    private:
        static_assert(std::is_arithmetic_v<ObjectT> || std::is_enum_v<ObjectT>,
            "The Object given to a NumericScribe was not arithmetic.");
    };

    template<class Object, class Archive>
    void NumericScribe<Object, Archive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            archive.AsOutput()->Write(object);
        else
            archive.AsInput()->Read(object);
    }

    template<class Archive>
    class Scribe<bool, Archive> : public NumericScribe<bool, Archive>
    {};

    template<class Archive>
    class Scribe<std::uint8_t, Archive> : public NumericScribe<std::uint8_t, Archive>
    {};

    template<class Archive>
    class Scribe<std::uint16_t, Archive> : public NumericScribe<std::uint16_t, Archive>
    {};

    template<class Archive>
    class Scribe<std::uint32_t, Archive> : public NumericScribe<std::uint32_t, Archive>
    {};

    template<class Archive>
    class Scribe<std::uint64_t, Archive> : public NumericScribe<std::uint64_t, Archive>
    {};

    template<class Archive>
    class Scribe<std::int8_t, Archive> : public NumericScribe<std::int8_t, Archive>
    {};

    template<class Archive>
    class Scribe<std::int16_t, Archive> : public NumericScribe<std::int16_t, Archive>
    {};

    template<class Archive>
    class Scribe<std::int32_t, Archive> : public NumericScribe<std::int32_t, Archive>
    {};

    template<class Archive>
    class Scribe<std::int64_t, Archive> : public NumericScribe<std::int64_t, Archive>
    {};

    template<class Archive>
    class Scribe<char, Archive> : public NumericScribe<char, Archive>
    {};

    template<class Archive>
    class Scribe<float, Archive> : public NumericScribe<float, Archive>
    {};

    template<class Archive>
    class Scribe<double, Archive> : public NumericScribe<double, Archive>
    {};
}