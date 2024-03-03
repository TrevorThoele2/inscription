#include <catch.hpp>

#include <Inscription/OptionalScribe.h>

#include "BinaryFixture.h"
#include "JsonFixture.h"

class OptionalBinaryTestsFixture : public BinaryFixture
{
public:
    using Integer = std::optional<int>;
};

class OptionalJsonTestsFixture : public JsonFixture
{
public:
    using Integer = std::optional<int>;
};

SCENARIO_METHOD(OptionalBinaryTestsFixture, "loading optional after save binary", "[binary][std][optional]")
{
    GIVEN("saved occupied optional")
    {
        Integer saved = dataGeneration.Random<int>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading optional")
        {
            Integer loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }

            THEN("is occupied")
            {
                REQUIRE(loaded.has_value());
            }
        }
    }

    GIVEN("saved unoccupied optional")
    {
        Integer saved;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading optional")
        {
            Integer loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }

            THEN("is not occupied")
            {
                REQUIRE(!loaded.has_value());
            }
        }
    }
}

SCENARIO_METHOD(OptionalJsonTestsFixture, "loading optional after save json", "[json][std][optional]")
{
    GIVEN("saved occupied optional")
    {
        Integer saved = dataGeneration.Random<int>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("optional", saved);
        }

        WHEN("loading optional")
        {
            Integer loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("optional", loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }

            THEN("is occupied")
            {
                REQUIRE(loaded.has_value());
            }
        }
    }

    GIVEN("saved unoccupied optional")
    {
        Integer saved;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("optional", saved);
        }

        WHEN("loading optional")
        {
            Integer loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("optional", loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }

            THEN("is not occupied")
            {
                REQUIRE(!loaded.has_value());
            }
        }
    }
}