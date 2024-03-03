#include <catch.hpp>

#include <Inscription/TupleScribe.h>
#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"

class TupleTestsFixture : public BinaryFixture
{};

SCENARIO_METHOD(TupleTestsFixture, "tuple loads after saving", "[binary][std][tuple]")
{
    using TestedObject = std::tuple<int, std::string, unsigned short>;

    GIVEN("saved tuple")
    {
        auto saved = dataGeneration.RandomStack<TestedObject, int, std::string, unsigned short>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading tuple")
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