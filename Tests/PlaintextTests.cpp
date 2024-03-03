#include <catch.hpp>

#include "GeneralFixture.h"

class PlaintextTestsFixture : public GeneralFixture
{};

SCENARIO_METHOD(PlaintextTestsFixture, "loading string in binary from file", "[binary][file]")
{
    GIVEN("saved string")
    {
        auto saved = dataGeneration.Random<std::string>();

        Inscription::Plaintext::ToFile(saved, "Test.txt");

        WHEN("loading")
        {
            const auto loaded = Inscription::Plaintext::FromFile("Test.txt");

            THEN("loaded is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved string with newline")
    {
        auto saved = "the\nstring";

        Inscription::Plaintext::ToFile(saved, "Test.txt");

        WHEN("loading")
        {
            const auto loaded = Inscription::Plaintext::FromFile("Test.txt");

            THEN("loaded is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}