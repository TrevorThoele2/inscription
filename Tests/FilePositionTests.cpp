#include <catch.hpp>

#include <Inscription/FilePositionScribe.h>

#include "GeneralFixture.h"

class FilePositionFixture : public GeneralFixture
{};

SCENARIO_METHOD(FilePositionFixture, "file position loads after save", "[binary][std][fileposition]")
{
    using TestedObject = Inscription::File::Position;

    GIVEN("saved stream position")
    {
        auto saved = TestedObject(dataGeneration.Random<unsigned long long>());

        Inscription::Binary::ToFile(saved, "Test.dat");

        WHEN("loading")
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