#include <catch.hpp>

#include <Inscription/ArrayScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class ArrayTestsFixture : public BinaryFixture
{};

SCENARIO_METHOD(ArrayTestsFixture, "loading array after saving", "[binary][container][array]")
{
    using TestedObject = std::array<int, 5>;

    GIVEN("saved array")
    {
        TestedObject saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(saved.size());

        for (size_t i = 0; i < startingGroup.size(); ++i)
            saved[i] = startingGroup[i];

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading array")
        {
            TestedObject loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as savd")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }
}