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
        NumericScribe() = delete;
        NumericScribe(const NumericScribe& arg) = delete;
        NumericScribe& operator=(const NumericScribe& arg) = delete;
    private:
        static void SaveImplementation(ObjectT& object, ArchiveT& archive, std::false_type isEnum);
        static void SaveImplementation(ObjectT& object, ArchiveT& archive, std::true_type isEnum);
        static void LoadImplementation(ObjectT& object, ArchiveT& archive);
    private:
        constexpr static bool isEnum = std::is_enum_v<ObjectT>;
    private:
        static_assert(std::is_arithmetic_v<ObjectT> || std::is_enum_v<ObjectT>, "The Object given to a NumericScribe was not arithmetic or an enum.");
    };

    template<class Object, class Archive>
    void NumericScribe<Object, Archive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive, std::bool_constant<isEnum>{});
        else
            LoadImplementation(object, archive);
    }

    template<class Object, class Archive>
    void NumericScribe<Object, Archive>::SaveImplementation(ObjectT& object, ArchiveT& archive, std::false_type isEnum)
    {
        archive.AsOutput()->WriteNumeric(object);
    }

    template<class Object, class Archive>
    void NumericScribe<Object, Archive>::SaveImplementation(ObjectT& object, ArchiveT& archive, std::true_type isEnum)
    {
        auto castedValue = static_cast<std::underlying_type_t<Object>>(object);
        archive.AsOutput()->WriteNumeric(castedValue);
    }

    template<class Object, class Archive>
    void NumericScribe<Object, Archive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive.AsInput()->ReadNumeric(object);
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