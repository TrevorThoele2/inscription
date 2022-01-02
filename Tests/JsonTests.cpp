#include <catch.hpp>

#include <Inscription/NumericScribe.h>

#include "GeneralFixture.h"

#include "Basics.h"
#include "TestClass.h"
#include "NamedClass.h"

class JsonTestsFixture : public GeneralFixture
{
public:
    Inscription::TypeRegistrationContext<Inscription::Format::Json> typeRegistrationContext;

    JsonTestsFixture()
    {
        typeRegistrationContext.RegisterType<TestClass>();
    }
};

SCENARIO_METHOD(JsonTestsFixture, "loading basics in json", "[json]")
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

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading")
        {
            Basics loaded = {};

            Inscription::Json::FromFile(loaded , "Test.json");

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
        auto saved = dataGeneration.RandomStack<TestClass, int, std::string>();

        Inscription::Json::ToFile(saved, "Test.json");

        THEN("loading")
        {
            TestClass loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

            THEN("is valid")
            {
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved class with colon in name")
    {
        auto saved = NamedClass{ "class:other", dataGeneration.Random<int>() };

        Inscription::Json::ToFile(saved, "Test.json");

        THEN("loading")
        {
            NamedClass loaded{ "class:other", 0 };

            Inscription::Json::FromFile(loaded, "Test.json");

            THEN("is valid")
            {
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved string with space in value")
    {
        const std::string saved = "the string";

        Inscription::Json::ToFile(saved, "Test.json");

        THEN("loading")
        {
            std::string loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

            THEN("is valid")
            {
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved string with newline in value")
    {
        const std::string saved = "the\nstring";

        Inscription::Json::ToFile(saved, "Test.json");

        THEN("loading")
        {
            std::string loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

            THEN("is valid")
            {
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved string with carriage return and line feed in value")
    {
        const std::string saved = "the\r\nstring";

        Inscription::Json::ToFile(saved, "Test.json");

        THEN("loading")
        {
            std::string loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

            THEN("is valid")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}