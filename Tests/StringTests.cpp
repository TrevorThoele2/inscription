#include <catch.hpp>

#include <Inscription/StringScribe.h>
#include <Inscription/ListScribe.h>

#include "BinaryFixture.h"
#include "JsonFixture.h"

class StringBinaryTestsFixture : public BinaryFixture
{};

class StringJsonTestsFixture : public JsonFixture
{};

SCENARIO_METHOD(StringBinaryTestsFixture, "loading string in binary", "[binary][std][string]")
{
    using TestedObject = std::string;

    GIVEN("saved string")
    {
        auto saved = dataGeneration.Random<TestedObject>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading string")
        {
            TestedObject loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}

SCENARIO_METHOD(StringJsonTestsFixture, "loading string in json", "[json][std][string]")
{
    using TestedObject = std::string;

    GIVEN("saved string")
    {
        auto saved = dataGeneration.Random<TestedObject>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("string", saved);
        }

        WHEN("loading string")
        {
            TestedObject loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("string", loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved string with sensitive character")
    {
        auto saved = GENERATE(std::string("abc:123"), std::string("abc\\123"), std::string("abc\"123"));

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("string", saved);
        }

        WHEN("loading string")
        {
            TestedObject loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("string", loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}

SCENARIO_METHOD(StringJsonTestsFixture, "loading string list in json", "[json][std][string][list]")
{
    using TestedObject = std::list<std::string>;

    GIVEN("saved string with sensitive character")
    {
        auto saved = TestedObject
        {
            std::string("abc:123"),
            std::string("abc\\123"),
            std::string("abc\"123")
        };

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("string", saved);
        }

        WHEN("loading string")
        {
            TestedObject loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("string", loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}