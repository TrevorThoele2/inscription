#include <catch.hpp>

#include <Inscription/MultiSetScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class MultiSetTestsFixture : public BinaryFixture
{};

SCENARIO_METHOD(MultiSetTestsFixture, "loading multiset after saving", "[binary][container][multiset]")
{
    using TestedObject = std::multiset<int>;

    GIVEN("saved multiset")
    {
        TestedObject saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);

        for (auto& loop : startingGroup)
            saved.emplace(loop);

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
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }
}