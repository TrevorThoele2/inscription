#include <catch.hpp>

#include <Inscription/VariantScribe.h>
#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"

class VariantTestsFixture : public BinaryFixture
{};

SCENARIO_METHOD(VariantTestsFixture, "variant loads after saving", "[binary][std][variant]")
{
    using TestedObject = std::variant<int, std::string, unsigned short>;

    GIVEN("saved tuple")
    {
        auto saved = dataGeneration.RandomStack<TestedObject, std::string>();

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