#include <catch.hpp>

#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"

class StringTestsFixture : public BinaryFixture
{};

SCENARIO_METHOD(StringTestsFixture, "string loads after save", "[binary][std][string]")
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