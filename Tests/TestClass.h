#pragma once

#include <string>

#include <Inscription/CompositeScribeCategory.h>
#include <Inscription/StringScribe.h>
#include <Inscription/NumericScribe.h>

class TestClass
{
public:
    int integer = 0;
    std::string string;

    TestClass() = default;
    TestClass(int integer, std::string string) : integer(integer), string(string)
    {}

    bool operator==(const TestClass& arg) const
    {
        return integer == arg.integer && string == arg.string;
    }

    bool operator<(const TestClass& arg) const
    {
        return integer < arg.integer && string < arg.string;
    }
};

namespace Inscription
{
    template<>
    class Scribe<TestClass> final
    {
    public:
        using ObjectT = TestClass;
    public:
        void Scriven(ObjectT& object, Format::Binary& format)
        {
            format(object.integer);
            format(object.string);
        }

        void Scriven(ObjectT& object, Format::Json& format)
        {
            format("integer", object.integer);
            format("string", object.string);
        }
    };

    template<class Format>
    struct ScribeTraits<TestClass, Format> final
    {
        using Category = CompositeScribeCategory<TestClass>;
    };
}

namespace std
{
    template<>
    struct hash<TestClass>
    {
        using argument_type = TestClass;
        using result_type = std::size_t;

        result_type operator()(const argument_type& arg) const
        {
            const result_type first(std::hash<string>()(arg.string));
            const result_type second(std::hash<int>()(arg.integer));
            return first ^ (second << 1);
        }
    };
}