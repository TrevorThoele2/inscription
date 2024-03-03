#include <catch.hpp>

#include <Inscription/NumericScribe.h>
#include <Inscription/BufferScribe.h>

#include "BinaryFixture.h"

#include "TestClass.h"

class BinaryTestsFixture : public BinaryFixture
{
public:
    BinaryTestsFixture()
    {
        typeRegistrationContext.RegisterType<TestClass>();
    }
};

SCENARIO_METHOD(BinaryTestsFixture, "opening input binary file", "[binary]")
{
    GIVEN("nonexistent file")
    {
        WHEN("loading directly")
        {
            auto inputFile = Inscription::File::InputBinary("Nonexistent.txt");

            THEN("position is 0")
            {
                REQUIRE(inputFile.Position() == 0);
            }

            THEN("size is 0")
            {
                REQUIRE(inputFile.Size() == 0);
            }
        }
    }
}

SCENARIO_METHOD(BinaryTestsFixture, "loading basics in binary", "[binary]")
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
            outputArchive(savedInt8);
            outputArchive(savedInt16);
            outputArchive(savedInt32);
            outputArchive(savedInt64);

            outputArchive(savedUint8);
            outputArchive(savedUint16);
            outputArchive(savedUint32);
            outputArchive(savedUint64);
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
                inputArchive(loadedInt8);
                inputArchive(loadedInt16);
                inputArchive(loadedInt32);
                inputArchive(loadedInt64);

                inputArchive(loadedUint8);
                inputArchive(loadedUint16);
                inputArchive(loadedUint32);
                inputArchive(loadedUint64);
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
        auto savedFloat = dataGeneration.Generator<float>().Random();
        auto savedDouble = dataGeneration.Generator<double>().Random();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(savedFloat);
            outputArchive(savedDouble);
        }

        WHEN("loading")
        {
            auto loadedFloat = 0.0f;
            auto loadedDouble = 0.0;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loadedFloat);
                inputArchive(loadedDouble);
            }

            THEN("is valid")
            {
                REQUIRE(loadedFloat == savedFloat);
                REQUIRE(loadedDouble == savedDouble);
            }
        }
    }

    GIVEN("saved class")
    {
        auto savedTestClass = dataGeneration.RandomStack<TestClass>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(savedTestClass);
        }

        THEN("loading")
        {
            TestClass loadedTestClass;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loadedTestClass);
            }

            THEN("is valid")
            {
                REQUIRE(loadedTestClass == savedTestClass);
            }
        }
    }

    GIVEN("saved empty")
    {
        {
            auto outputArchive = CreateRegistered<OutputArchive>();
        }

        THEN("loading empty buffer")
        {
            Inscription::Buffer buffer;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(buffer);
            }

            THEN("buffer is still empty")
            {
                REQUIRE(buffer.value.empty());
            }
        }
    }
}