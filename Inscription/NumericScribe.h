#pragma once

#include "Scribe.h"

#include "OutputBinaryFormat.h"
#include "InputBinaryFormat.h"
#include "OutputJsonFormat.h"
#include "InputJsonFormat.h"

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
        static void Scriven(ObjectT& object, Format::Binary& format);
        static void Scriven(const std::string& name, ObjectT& object, Format::Json& format);
    };

    template<class Object>
    void NumericScribeCategory<Object>::Scriven(ObjectT& object, Format::Binary& format)
    {
        if (format.IsOutput())
            format.AsOutput()->Write(object);
        else
            format.AsInput()->Read(object);
    }

    template<class Object>
    void NumericScribeCategory<Object>::Scriven(const std::string& name, ObjectT& object, Format::Json& format)
    {
        if (format.IsOutput())
            format.AsOutput()->WriteValue(name, Chroma::ToString(object));
        else
        {
            std::string output;
            format.AsInput()->TakeValue(name, output);
            auto value = Chroma::FromString<ObjectT>(output);
            new (&object) ObjectT(value);
        }
    }

    template<class Format>
    struct ScribeTraits<bool, Format> final
    {
        using Category = NumericScribeCategory<bool>;
        static constexpr bool shouldTrack = false;
    };

    template<class Format>
    struct ScribeTraits<uint8_t, Format> final
    {
        using Category = NumericScribeCategory<uint8_t>;
        static constexpr bool shouldTrack = false;
    };

    template<class Format>
    struct ScribeTraits<uint16_t, Format> final
    {
        using Category = NumericScribeCategory<uint16_t>;
        static constexpr bool shouldTrack = false;
    };

    template<class Format>
    struct ScribeTraits<uint32_t, Format> final
    {
        using Category = NumericScribeCategory<uint32_t>;
        static constexpr bool shouldTrack = false;
    };

    template<class Format>
    struct ScribeTraits<uint64_t, Format> final
    {
        using Category = NumericScribeCategory<uint64_t>;
        static constexpr bool shouldTrack = false;
    };

    template<class Format>
    struct ScribeTraits<int8_t, Format> final
    {
        using Category = NumericScribeCategory<int8_t>;
        static constexpr bool shouldTrack = false;
    };

    template<class Format>
    struct ScribeTraits<int16_t, Format> final
    {
        using Category = NumericScribeCategory<int16_t>;
        static constexpr bool shouldTrack = false;
    };

    template<class Format>
    struct ScribeTraits<int32_t, Format> final
    {
        using Category = NumericScribeCategory<int32_t>;
        static constexpr bool shouldTrack = false;
    };

    template<class Format>
    struct ScribeTraits<int64_t, Format> final
    {
        using Category = NumericScribeCategory<int64_t>;
        static constexpr bool shouldTrack = false;
    };

    template<class Format>
    struct ScribeTraits<char, Format> final
    {
        using Category = NumericScribeCategory<char>;
        static constexpr bool shouldTrack = false;
    };

    template<class Format>
    struct ScribeTraits<float, Format> final
    {
        using Category = NumericScribeCategory<float>;
        static constexpr bool shouldTrack = false;
    };

    template<class Format>
    struct ScribeTraits<double, Format> final
    {
        using Category = NumericScribeCategory<double>;
        static constexpr bool shouldTrack = false;
    };
}