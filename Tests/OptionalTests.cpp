#include <catch.hpp>

#include <Inscription/OptionalScribe.h>

#include "BinaryFixture.h"

class OptionalTestsFixture : public BinaryFixture
{};

SCENARIO_METHOD(OptionalTestsFixture, "optional loads after save", "[binary][std][optional]")
{
    using TestedObject = std::optional<int>;

    GIVEN("saved occupied optional")
    {
        std::optional<int> saved = dataGeneration.Random<int>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading optional")
        {
            TestedObject loaded;

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
        std::optional<int> saved;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading optional")
        {
            TestedObject loaded;

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