#include <catch.hpp>

#include <Inscription/StringScribe.h>
#include <Inscription/ListScribe.h>

#include "GeneralFixture.h"

class StringTestsFixture : public GeneralFixture
{};

SCENARIO_METHOD(StringTestsFixture, "loading string in binary", "[binary][std][string]")
{
    using TestedObject = std::string;

    GIVEN("saved string")
    {
        auto saved = dataGeneration.Random<TestedObject>();

        Inscription::Binary::ToFile(saved, "Test.dat");

        WHEN("loading string")
        {
            TestedObject loaded;

            Inscription::Binary::FromFile(loaded, "Test.dat");

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}

SCENARIO_METHOD(StringTestsFixture, "loading string in json", "[json][std][string]")
{
    using TestedObject = std::string;

    GIVEN("saved string")
    {
        auto saved = dataGeneration.Random<TestedObject>();

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading string")
        {
            TestedObject loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved string with sensitive character")
    {
        auto saved = GENERATE(std::string("abc:123"), std::string("abc\\123"), std::string("abc\\\"123"));

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading string")
        {
            TestedObject loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}

SCENARIO_METHOD(StringTestsFixture, "loading string list in json", "[json][std][string][list]")
{
    using TestedObject = std::list<std::string>;

    GIVEN("saved string with sensitive character")
    {
        auto saved = TestedObject
        {
            std::string("abc:123"),
            std::string("abc\\123"),
            std::string("abc\\\"123")
        };

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading string")
        {
            TestedObject loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}