#include <catch.hpp>

#include <Inscription/NumericScribe.h>
#include <Inscription/BufferScribe.h>

#include "GeneralFixture.h"

#include "TestClass.h"
#include "Basics.h"

class BinaryTestsFixture : public GeneralFixture
{
public:
    Inscription::TypeRegistrationContext<Inscription::Format::Binary> typeRegistrationContext;

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
        auto saved = dataGeneration.RandomStack<
            Basics,
            std::int8_t,
            std::int16_t,
            std::int32_t,
            std::int64_t,
            std::uint8_t,
            std::uint16_t,
            std::uint32_t,
            std::uint64_t,
            float,
            double>();

        Inscription::Binary::ToFile(saved, "Test.dat", typeRegistrationContext);

        WHEN("loading")
        {
            Basics loaded = {};

            Inscription::Binary::FromFile(loaded, "Test.dat", typeRegistrationContext);

            THEN("all valid")
            {
                REQUIRE(loaded.int8 == saved.int8);
                REQUIRE(loaded.int16 == saved.int16);
                REQUIRE(loaded.int32 == saved.int32);
                REQUIRE(loaded.int64 == saved.int64);
                REQUIRE(loaded.uint8 == saved.uint8);
                REQUIRE(loaded.uint16 == saved.uint16);
                REQUIRE(loaded.uint32 == saved.uint32);
                REQUIRE(loaded.uint64 == saved.uint64);
                REQUIRE(loaded.floatValue == Approx(saved.floatValue));
                REQUIRE(loaded.doubleValue == Approx(saved.doubleValue));
            }
        }
    }
    
    GIVEN("saved class")
    {
        auto saved = dataGeneration.RandomStack<TestClass>();

        Inscription::Binary::ToFile(saved, "Test.dat", typeRegistrationContext);

        THEN("loading")
        {
            TestClass loaded;

            Inscription::Binary::FromFile(loaded, "Test.dat", typeRegistrationContext);

            THEN("is valid")
            {
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved empty")
    {
        {
            auto file = Inscription::File::OutputBinary("Test.dat");
            auto archive = Inscription::Archive::OutputBinary(file);
            auto format = Inscription::Format::OutputBinary(archive, typeRegistrationContext);
        }

        THEN("loading empty buffer")
        {
            Inscription::Buffer buffer;

            Inscription::Binary::FromFile(buffer, "Test.dat", typeRegistrationContext);

            THEN("buffer is still empty")
            {
                REQUIRE(buffer.value.empty());
            }
        }
    }
}