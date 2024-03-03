#include <catch.hpp>

#include <Inscription/NumericScribe.h>

#include "JsonFixture.h"

class JsonParseErrorTestsFixture : public JsonFixture
{};

SCENARIO_METHOD(JsonParseErrorTestsFixture, "json parse errors", "[json][parse][error]")
{
    GIVEN("saved nothing")
    {
        {
            auto outputArchive = CreateRegistered<OutputArchive>();
        }

        WHEN("loading value")
        {
            auto inputArchive = CreateRegistered<InputArchive>();

            THEN("throws exception")
            {
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

    GIVEN("saved list element with name")
    {
        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive.StartList("list");

            auto element = 0;
            outputArchive("element", element);

            outputArchive.EndList();
        }

        WHEN("loading")
        {
            auto inputArchive = CreateRegistered<InputArchive>();

            auto element = 0;

            inputArchive.StartList("list");
            THEN("throw exception")
            {
                REQUIRE_THROWS_MATCHES
                (
                    inputArchive("element", element),
                    Inscription::JsonParseError,
                    Catch::Matchers::Message(
                        "The element \"element\" could not be found.")
                );
            }
        }
    }

    GIVEN("saved value element on object without name")
    {
        {
            auto outputArchive = CreateRegistered<OutputTextArchive>();
            outputArchive.Write(std::string("{1}"));
        }

        WHEN("creating archive")
        {
            THEN("throw exception")
            {
                REQUIRE_THROWS_MATCHES
                (
                    CreateRegistered<InputArchive>(),
                    Inscription::JsonParseError,
                    Catch::Matchers::Message(
                        "Value on object requires name.")
                );
            }
        }
    }

    GIVEN("saved list element on object without name")
    {
        {
            auto outputArchive = CreateRegistered<OutputTextArchive>();
            outputArchive.Write(std::string("{[]}"));
        }

        WHEN("creating archive")
        {
            THEN("throws exception")
            {
                REQUIRE_THROWS_MATCHES
                (
                    CreateRegistered<InputArchive>(),
                    Inscription::JsonParseError,
                    Catch::Matchers::Message(
                        "Value on object requires name.")
                );
            }
        }
    }

    GIVEN("saved object element on object without name")
    {
        {
            auto outputArchive = CreateRegistered<OutputTextArchive>();
            outputArchive.Write(std::string("{{}}"));
        }

        WHEN("creating archive")
        {
            THEN("throws exception")
            {
                REQUIRE_THROWS_MATCHES
                (
                    CreateRegistered<InputArchive>(),
                    Inscription::JsonParseError,
                    Catch::Matchers::Message(
                        "Value on object requires name.")
                );
            }
        }
    }

    GIVEN("saved list with name like object")
    {
        {
            auto outputArchive = CreateRegistered<OutputTextArchive>();
            outputArchive.Write(std::string("{[\"element\": 1]}"));
        }

        WHEN("creating archive")
        {
            THEN("throws exception")
            {
                REQUIRE_THROWS_MATCHES
                (
                    CreateRegistered<InputArchive>(),
                    Inscription::JsonParseError,
                    Catch::Matchers::Message(
                        "Value on object requires name.")
                );
            }
        }
    }
}