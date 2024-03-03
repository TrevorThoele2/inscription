#include <boost/test/unit_test.hpp>

#include "BinaryFixture.h"

class BinarySerializationTestsFixture : public BinaryFixture
{
public:
    ::TestFramework::DataGeneration dataGeneration;

    BinarySerializationTestsFixture()
    {
        typeRegistrationContext.RegisterType<TestClass>();
    }

    class TestClass
    {
    public:
        TestClass(int value = 0) : value(value)
        {}

        int Value() const
        {
            return value;
        }
    private:
        int value;
    private:
        INSCRIPTION_BINARY_SERIALIZE_FUNCTION
        {
            scribe(value);
        }

        INSCRIPTION_ACCESS;
    };

    TestClass CreateRandomTestClass()
    {
        return dataGeneration.Generator<TestClass>().RandomStack<int>();
    }

    TestClass* CreateRandomHeapTestClass()
    {
        return dataGeneration.Generator<TestClass>().RandomHeap<int>();
    }
};

BOOST_FIXTURE_TEST_SUITE(BinarySerializationTests, BinarySerializationTestsFixture)

BOOST_AUTO_TEST_CASE(Integer_SavesAndLoads)
{
    std::int8_t int8 = dataGeneration.Generator<std::int8_t>().Random();
    std::int16_t int16 = dataGeneration.Generator<std::int16_t>().Random();
    std::int32_t int32 = dataGeneration.Generator<std::int32_t>().Random();
    std::int64_t int64 = dataGeneration.Generator<std::int64_t>().Random();

    std::uint8_t uint8 = dataGeneration.Generator<std::uint8_t>().Random();
    std::uint16_t uint16 = dataGeneration.Generator<std::uint16_t>().Random();
    std::uint32_t uint32 = dataGeneration.Generator<std::uint32_t>().Random();
    std::uint64_t uint64 = dataGeneration.Generator<std::uint64_t>().Random();

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(int8);
        outputScribe.Save(int16);
        outputScribe.Save(int32);
        outputScribe.Save(int64);

        outputScribe.Save(uint8);
        outputScribe.Save(uint16);
        outputScribe.Save(uint32);
        outputScribe.Save(uint64);
    }

    std::int8_t n_int8 = 0;
    std::int16_t n_int16 = 0;
    std::int32_t n_int32 = 0;
    std::int64_t n_int64 = 0;

    std::uint8_t n_uint8 = 0;
    std::uint16_t n_uint16 = 0;
    std::uint32_t n_uint32 = 0;
    std::uint64_t n_uint64 = 0;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_int8);
        inputScribe.Load(n_int16);
        inputScribe.Load(n_int32);
        inputScribe.Load(n_int64);

        inputScribe.Load(n_uint8);
        inputScribe.Load(n_uint16);
        inputScribe.Load(n_uint32);
        inputScribe.Load(n_uint64);
    }

    BOOST_REQUIRE(n_int8 == int8);
    BOOST_REQUIRE(n_int16 == int16);
    BOOST_REQUIRE(n_int32 == int32);
    BOOST_REQUIRE(n_int64 == int64);

    BOOST_REQUIRE(n_uint8 == uint8);
    BOOST_REQUIRE(n_uint16 == uint16);
    BOOST_REQUIRE(n_uint32 == uint32);
    BOOST_REQUIRE(n_uint64 == uint64);
}

BOOST_AUTO_TEST_CASE(FloatingPoint_SavesAndLoads)
{
    float f = dataGeneration.Generator<float>().Random();
    double d = dataGeneration.Generator<double>().Random();

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(f);
        outputScribe.Save(d);
    }

    float n_f = 0.0f;
    double n_d = 0.0;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_f);
        inputScribe.Load(n_d);
    }

    BOOST_REQUIRE(n_f == f);
    BOOST_REQUIRE(n_d == d);
}

BOOST_AUTO_TEST_CASE(Class_SavesAndLoads)
{
    auto testClass = CreateRandomTestClass();

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(testClass);
    }

    TestClass n_testClass(0);

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_testClass);
    }

    BOOST_REQUIRE(n_testClass.Value() == testClass.Value());
}

BOOST_AUTO_TEST_CASE(UnowningPointer_SavesAndLoads)
{
    auto testClassFirst = CreateRandomTestClass();
    auto testClassSecond = CreateRandomTestClass();

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.SaveUnowningPointer(&testClassFirst);
        outputScribe.Save(testClassFirst);

        outputScribe.Save(testClassSecond);
        outputScribe.SaveUnowningPointer(&testClassSecond);
    }

    TestClass* n_testClassFirst;
    TestClass n_testClassFirstBacking;
    TestClass* n_testClassSecond;
    TestClass n_testClassSecondBacking;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.LoadUnowningPointer(n_testClassFirst);
        inputScribe.Load(n_testClassFirstBacking);

        inputScribe.Load(n_testClassSecondBacking);
        inputScribe.LoadUnowningPointer(n_testClassSecond);
    }

    BOOST_REQUIRE(n_testClassFirst->Value() == testClassFirst.Value());
    BOOST_REQUIRE(n_testClassSecond->Value() == testClassSecond.Value());
}

BOOST_AUTO_TEST_CASE(OwningPointer_SavesAndLoads)
{
    auto testClass = CreateRandomHeapTestClass();

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.SaveOwningPointer(testClass);
    }

    TestClass* n_testClass;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.LoadOwningPointer(n_testClass);
    }

    BOOST_REQUIRE(n_testClass->Value() == testClass->Value());

    delete testClass;
    delete n_testClass;
}

BOOST_AUTO_TEST_CASE(OwningAndUnowningPointers_SavesAndLoads)
{
    auto testClassFirst = CreateRandomHeapTestClass();
    auto testClassSecond = CreateRandomHeapTestClass();

    TestClass* testClass_unowning_first = testClassFirst;
    TestClass* testClass_unowning_second = testClassSecond;

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.SaveUnowningPointer(testClass_unowning_first);
        outputScribe.SaveOwningPointer(testClassFirst);

        outputScribe.SaveOwningPointer(testClassSecond);
        outputScribe.SaveUnowningPointer(testClass_unowning_second);
    }

    TestClass* n_testClassFirst;
    TestClass* n_testClassSecond;

    TestClass* n_testClass_unowning_first;
    TestClass* n_testClass_unowning_second;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.LoadUnowningPointer(n_testClass_unowning_first);
        inputScribe.LoadOwningPointer(n_testClassFirst);

        inputScribe.LoadOwningPointer(n_testClassSecond);
        inputScribe.LoadUnowningPointer(n_testClass_unowning_second);
    }

    BOOST_REQUIRE(n_testClass_unowning_first->Value() == testClass_unowning_first->Value());
    BOOST_REQUIRE(n_testClass_unowning_second->Value() == testClass_unowning_second->Value());

    delete testClassFirst;
    delete testClassSecond;
    delete n_testClassFirst;
    delete n_testClassSecond;
}

BOOST_AUTO_TEST_SUITE_END()