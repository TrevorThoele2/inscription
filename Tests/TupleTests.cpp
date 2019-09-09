#include <catch.hpp>

#include <Inscription/TupleScribe.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"

class TupleTestsFixture : public BinaryFixture
{};

TEST_CASE_METHOD(TupleTestsFixture, "std::tuple")
{
    using TestedObject = std::tuple<int, std::string, unsigned short>;

    SECTION("save")
    {
        auto saved = dataGeneration.Generator<TestedObject>().RandomStack<int, std::string, unsigned short>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        SECTION("load")
        {
            TestedObject loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            REQUIRE(loaded == saved);
        }
    }
}