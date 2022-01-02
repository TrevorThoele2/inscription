#include <catch.hpp>

#include <Inscription/OptionalScribe.h>

#include "GeneralFixture.h"

class OptionalTestsFixture : public GeneralFixture
{
public:
    using Integer = std::optional<int>;
};

SCENARIO_METHOD(OptionalTestsFixture, "loading optional after save binary", "[binary][std][optional]")
{
    GIVEN("saved occupied optional")
    {
        Integer saved = dataGeneration.Random<int>();

        Inscription::Binary::ToFile(saved, "Test.dat");

        WHEN("loading optional")
        {
            Integer loaded;

            Inscription::Binary::FromFile(loaded, "Test.dat");

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

        Inscription::Binary::ToFile(saved, "Test.dat");

        WHEN("loading optional")
        {
            Integer loaded;

            Inscription::Binary::FromFile(loaded, "Test.dat");

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

SCENARIO_METHOD(OptionalTestsFixture, "loading optional after save json", "[json][std][optional]")
{
    GIVEN("saved occupied optional")
    {
        Integer saved = dataGeneration.Random<int>();

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading optional")
        {
            Integer loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

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

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading optional")
        {
            Integer loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

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