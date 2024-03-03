#pragma once

#include <string>

#include <Inscription/CompositeScribeCategory.h>
#include <Inscription/StringScribe.h>
#include <Inscription/NumericScribe.h>

class Basics
{
public:
    std::int8_t int8;
    std::int16_t int16;
    std::int32_t int32;
    std::int64_t int64;
    std::uint8_t uint8;
    std::uint16_t uint16;
    std::uint32_t uint32;
    std::uint64_t uint64;
    float floatValue;
    double doubleValue;

    Basics() = default;
    Basics(
        std::int8_t int8,
	    std::int16_t int16,
	    std::int32_t int32,
	    std::int64_t int64,
	    std::uint8_t uint8,
	    std::uint16_t uint16,
	    std::uint32_t uint32,
	    std::uint64_t uint64,
	    float floatValue,
	    double doubleValue)
		:
		int8(int8),
        int16(int16),
		int32(int32),
	    int64(int64),
	    uint8(uint8),
	    uint16(uint16),
	    uint32(uint32),
	    uint64(uint64),
	    floatValue(floatValue),
		doubleValue(doubleValue)
    {}
};

namespace Inscription
{
    template<>
    class Scribe<Basics> final
    {
    public:
        using ObjectT = Basics;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format("int8", object.int8);
            format("int16", object.int16);
            format("int32", object.int32);
            format("int64", object.int64);
            format("uint8", object.uint8);
            format("uint16", object.uint16);
            format("uint32", object.uint32);
            format("uint64", object.uint64);
            format("float", object.floatValue);
            format("double", object.doubleValue);
        }
    };

    template<class Format>
    struct ScribeTraits<Basics, Format> final
    {
        using Category = CompositeScribeCategory<Basics>;
    };
}