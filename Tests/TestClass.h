#pragma once

#include <string>

#include <Inscription/CompositeScribe.h>

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
    class Scribe<TestClass, BinaryArchive> final :
        public CompositeScribe<TestClass, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive(object.integer);
            archive(object.string);
        }
    };

    template<>
    class Scribe<TestClass, JsonArchive> final :
        public CompositeScribe<TestClass, JsonArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive("integer", object.integer);
            archive("string", object.string);
        }
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