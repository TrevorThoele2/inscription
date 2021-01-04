#include <catch.hpp>

#include <Inscription/NumericScribe.h>

#include "JsonFixture.h"

#include "TestClass.h"

class JsonTestsFixture : public JsonFixture
{
public:
    JsonTestsFixture()
    {
        typeRegistrationContext.RegisterType<TestClass>();
    }
};

SCENARIO_METHOD(JsonTestsFixture, "loading from empty file in json", "[json]")
{
    GIVEN("saved nothing")
    {
        {
            auto outputArchive = CreateRegistered<OutputArchive>();
        }

        WHEN("creating input archive")
        {
            THEN("does not throw exception")
            {
                REQUIRE_NOTHROW(CreateRegistered<InputArchive>());
            }
        }

        WHEN("loading value")
        {
            THEN("throws exception")
            {
                auto inputArchive = CreateRegistered<InputArchive>();

                std::int8_t value;

                REQUIRE_THROWS_MATCHES
                (
                    inputArchive("nonexistent", value),
                    Inscription::JsonParseError,
                    Catch::Matchers::Message(
                        "The element \"nonexistent\" could not be found.")
                );
            }
        }
    }
}

SCENARIO_METHOD(JsonTestsFixture, "loading basics in json", "[json]")
{
    GIVEN("saved all integers")
    {
        auto savedInt8 = dataGeneration.Generator<std::int8_t>().Random();
        auto savedInt16 = dataGeneration.Generator<std::int16_t>().Random();
        auto savedInt32 = dataGeneration.Generator<std::int32_t>().Random();
        auto savedInt64 = dataGeneration.Generator<std::int64_t>().Random();

        auto savedUint8 = dataGeneration.Generator<std::uint8_t>().Random();
        auto savedUint16 = dataGeneration.Generator<std::uint16_t>().Random();
        auto savedUint32 = dataGeneration.Generator<std::uint32_t>().Random();
        auto savedUint64 = dataGeneration.Generator<std::uint64_t>().Random();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("int8", savedInt8);
            outputArchive("int16", savedInt16);
            outputArchive("int32", savedInt32);
            outputArchive("int64", savedInt64);

            outputArchive("uint8", savedUint8);
            outputArchive("uint16", savedUint16);
            outputArchive("uint32", savedUint32);
            outputArchive("uint64", savedUint64);
        }

        WHEN("loading")
        {
            std::int8_t loadedInt8 = 0;
            std::int16_t loadedInt16 = 0;
            auto loadedInt32 = 0;
            std::int64_t loadedInt64 = 0;

            std::uint8_t loadedUint8 = 0;
            std::uint16_t loadedUint16 = 0;
            std::uint32_t loadedUint32 = 0;
            std::uint64_t loadedUint64 = 0;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive("int8", loadedInt8);
                inputArchive("int16", loadedInt16);
                inputArchive("int32", loadedInt32);
                inputArchive("int64", loadedInt64);

                inputArchive("uint8", loadedUint8);
                inputArchive("uint16", loadedUint16);
                inputArchive("uint32", loadedUint32);
                inputArchive("uint64", loadedUint64);
            }

            THEN("all valid")
            {
                REQUIRE(loadedInt8 == savedInt8);
                REQUIRE(loadedInt16 == savedInt16);
                REQUIRE(loadedInt32 == savedInt32);
                REQUIRE(loadedInt64 == savedInt64);

                REQUIRE(loadedUint8 == savedUint8);
                REQUIRE(loadedUint16 == savedUint16);
                REQUIRE(loadedUint32 == savedUint32);
                REQUIRE(loadedUint64 == savedUint64);
            }
        }
    }

    GIVEN("saved floating points")
    {
        auto savedFloat = dataGeneration.Generator<float>().Random(TestFramework::Range<float>(-1000, 1000));
        auto savedDouble = dataGeneration.Generator<double>().Random(TestFramework::Range<double>(-1000, 1000));

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("float", savedFloat);
            outputArchive("double", savedDouble);
        }

        WHEN("loading")
        {
            auto loadedFloat = 0.0f;
            auto loadedDouble = 0.0;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive("float", loadedFloat);
                inputArchive("double", loadedDouble);
            }

            THEN("is valid")
            {
                REQUIRE(loadedFloat == Approx(savedFloat));
                REQUIRE(loadedDouble == Approx(savedDouble));
            }
        }
    }

    GIVEN("saved class")
    {
        auto savedTestClass = dataGeneration.RandomStack<TestClass, int, std::string>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("class", savedTestClass);
        }

        THEN("loading")
        {
            TestClass loadedTestClass;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive("class", loadedTestClass);
            }

            THEN("is valid")
            {
                REQUIRE(loadedTestClass == savedTestClass);
            }
        }
    }

    GIVEN("saved class with colon in name")
    {
        auto savedTestClass = dataGeneration.RandomStack<TestClass, int, std::string>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("class:other", savedTestClass);
        }

        THEN("loading")
        {
            TestClass loadedTestClass;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive("class:other", loadedTestClass);
            }

            THEN("is valid")
            {
                REQUIRE(loadedTestClass == savedTestClass);
            }
        }
    }

    GIVEN("saved string with space in value")
    {
        const std::string savedString = "the string";

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("str", savedString);
        }

        THEN("loading")
        {
            std::string loadedString;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive("str", loadedString);
            }

            THEN("is valid")
            {
                REQUIRE(loadedString == savedString);
            }
        }
    }

    GIVEN("saved string with newline in value")
    {
        const std::string savedString = "the\nstring";

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("str", savedString);
        }

        THEN("loading")
        {
            std::string loadedString;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive("str", loadedString);
            }

            THEN("is valid")
            {
                REQUIRE(loadedString == savedString);
            }
        }
    }

    GIVEN("saved string with carriage return and line feed in value")
    {
        const std::string savedString = "the\r\nstring";

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("str", savedString);
        }

        THEN("loading")
        {
            std::string loadedString;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive("str", loadedString);
            }

            THEN("is valid")
            {
                REQUIRE(loadedString == savedString);
            }
        }
    }
}