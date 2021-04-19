#pragma once

#include "Scribe.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"
#include "OutputJsonArchive.h"
#include "InputJsonArchive.h"

#include <Chroma/StringUtility.h>

namespace Inscription
{
    template<class Object>
    class NumericScribeCategory final
    {
    public:
        using ObjectT = Object;
    public:
        static constexpr bool requiresScribe = false;
    public:
        static void Scriven(ObjectT& object, Archive::Binary& archive);
        static void Scriven(const std::string& name, ObjectT& object, Archive::Json& archive);
    };

    template<class Object>
    void NumericScribeCategory<Object>::Scriven(ObjectT& object, Archive::Binary& archive)
    {
        if (archive.IsOutput())
            archive.AsOutput()->Write(object);
        else
            archive.AsInput()->Read(object);
    }

    template<class Object>
    void NumericScribeCategory<Object>::Scriven(const std::string& name, ObjectT& object, Archive::Json& archive)
    {
        if (archive.IsOutput())
            archive.AsOutput()->WriteValue(name, Chroma::ToString(object));
        else
        {
            std::string output;
            archive.AsInput()->TakeValue(name, output);
            auto value = Chroma::FromString<ObjectT>(output);
            new (&object) ObjectT(value);
        }
    }

    template<class Archive>
    struct ScribeTraits<bool, Archive> final
    {
        using Category = NumericScribeCategory<bool>;
        static constexpr bool shouldTrack = false;
    };

    template<class Archive>
    struct ScribeTraits<uint8_t, Archive> final
    {
        using Category = NumericScribeCategory<uint8_t>;
        static constexpr bool shouldTrack = false;
    };

    template<class Archive>
    struct ScribeTraits<uint16_t, Archive> final
    {
        using Category = NumericScribeCategory<uint16_t>;
        static constexpr bool shouldTrack = false;
    };

    template<class Archive>
    struct ScribeTraits<uint32_t, Archive> final
    {
        using Category = NumericScribeCategory<uint32_t>;
        static constexpr bool shouldTrack = false;
    };

    template<class Archive>
    struct ScribeTraits<uint64_t, Archive> final
    {
        using Category = NumericScribeCategory<uint64_t>;
        static constexpr bool shouldTrack = false;
    };

    template<class Archive>
    struct ScribeTraits<int8_t, Archive> final
    {
        using Category = NumericScribeCategory<int8_t>;
        static constexpr bool shouldTrack = false;
    };

    template<class Archive>
    struct ScribeTraits<int16_t, Archive> final
    {
        using Category = NumericScribeCategory<int16_t>;
        static constexpr bool shouldTrack = false;
    };

    template<class Archive>
    struct ScribeTraits<int32_t, Archive> final
    {
        using Category = NumericScribeCategory<int32_t>;
        static constexpr bool shouldTrack = false;
    };

    template<class Archive>
    struct ScribeTraits<int64_t, Archive> final
    {
        using Category = NumericScribeCategory<int64_t>;
        static constexpr bool shouldTrack = false;
    };

    template<class Archive>
    struct ScribeTraits<char, Archive> final
    {
        using Category = NumericScribeCategory<char>;
        static constexpr bool shouldTrack = false;
    };

    template<class Archive>
    struct ScribeTraits<float, Archive> final
    {
        using Category = NumericScribeCategory<float>;
        static constexpr bool shouldTrack = false;
    };

    template<class Archive>
    struct ScribeTraits<double, Archive> final
    {
        using Category = NumericScribeCategory<double>;
        static constexpr bool shouldTrack = false;
    };
}