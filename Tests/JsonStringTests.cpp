#include <catch.hpp>

#include <Inscription/NumericScribe.h>

#include "GeneralFixture.h"

#include "TestClass.h"
#include "Basics.h"
#include "NamedClass.h"

class JsonStringTestsFixture : public GeneralFixture
{
public:
    Inscription::TypeRegistrationContext<Inscription::Format::Json> typeRegistrationContext;

    JsonStringTestsFixture()
    {
        typeRegistrationContext.RegisterType<TestClass>();
    }
};

SCENARIO_METHOD(JsonStringTestsFixture, "loading from empty string in json", "[json]")
{
    GIVEN("empty json")
    {
        const std::string json;

        WHEN("loading from string")
        {
            std::string value;

            THEN("throws exception")
            {
                REQUIRE_THROWS_MATCHES(
                    Inscription::Json::FromString(value, json),
                    Inscription::JsonParseError,
                    Catch::Matchers::Message(
                        "The element \"\" could not be found."));
            }
        }
    }
}

SCENARIO_METHOD(JsonStringTestsFixture, "loading basics from string in json", "[json]")
{
    std::string savedJson;

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

        savedJson = Inscription::Json::ToString(saved, typeRegistrationContext);

        WHEN("loading")
        {
            Basics loaded = {};

            Inscription::Json::FromString(loaded, savedJson, typeRegistrationContext);
            
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

        savedJson = Inscription::Json::ToString(saved, typeRegistrationContext);

        THEN("loading")
        {
            TestClass loaded;

            Inscription::Json::FromString(loaded, savedJson, typeRegistrationContext);

            THEN("is valid")
            {
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved class with colon in name")
    {
        auto saved = NamedClass{ "class:other", dataGeneration.Random<int>() };

        savedJson = Inscription::Json::ToString(saved, typeRegistrationContext);

        THEN("loading")
        {
            NamedClass loaded{ "class:other", 0 };

            Inscription::Json::FromString(loaded, savedJson, typeRegistrationContext);

            THEN("is valid")
            {
                REQUIRE(loaded.value == saved.value);
            }
        }
    }

    GIVEN("saved string with space in value")
    {
        auto saved = TestClass{ dataGeneration.Random<int>(), "the string" };

        savedJson = Inscription::Json::ToString(saved, typeRegistrationContext);

        THEN("loading")
        {
            TestClass loaded;

            Inscription::Json::FromString(loaded, savedJson, typeRegistrationContext);

            THEN("is valid")
            {
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved string with newline in value")
    {
        auto saved = TestClass{ dataGeneration.Random<int>(), "the\nstring" };

        savedJson = Inscription::Json::ToString(saved, typeRegistrationContext);

        THEN("loading")
        {
            TestClass loaded;

            Inscription::Json::FromString(loaded, savedJson, typeRegistrationContext);

            THEN("is valid")
            {
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved string with carriage return and line feed in value")
    {
        auto saved = TestClass{ dataGeneration.Random<int>(), "the\r\nstring" };

        savedJson = Inscription::Json::ToString(saved, typeRegistrationContext);

        THEN("loading")
        {
            TestClass loaded;

            Inscription::Json::FromString(loaded, savedJson, typeRegistrationContext);

            THEN("is valid")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}