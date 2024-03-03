#include <catch.hpp>

#include <Inscription/FilePositionScribe.h>

#include "BinaryFixture.h"

class FilePositionFixture : public BinaryFixture
{};

SCENARIO_METHOD(FilePositionFixture, "file position loads after save", "[binary][std][fileposition]")
{
    using TestedObject = Inscription::File::Position;

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