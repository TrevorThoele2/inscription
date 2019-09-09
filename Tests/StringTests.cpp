#include <catch.hpp>

#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"

class StringTestsFixture : public BinaryFixture
{};

TEST_CASE_METHOD(StringTestsFixture, "std::string")
{
    using TestedObject = std::string;

    SECTION("save")
    {
        auto saved = dataGeneration.Generator<TestedObject>().Random();

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