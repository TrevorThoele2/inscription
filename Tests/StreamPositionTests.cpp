#include <catch.hpp>

#include <Inscription/StreamPositionScribe.h>

#include "BinaryFixture.h"

class StreamPositionFixture : public BinaryFixture
{};

SCENARIO_METHOD(StreamPositionFixture, "stream position loads after save", "[binary][std][streamposition]")
{
    using TestedObject = Inscription::StreamPosition;

    GIVEN("saved stream position")
    {
        auto saved = TestedObject(dataGeneration.Random<unsigned long long>());

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
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