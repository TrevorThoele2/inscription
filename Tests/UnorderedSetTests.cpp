#include <catch.hpp>

#include <Inscription/UnorderedSetScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class UnorderedSetTestsFixture : public BinaryFixture
{};

SCENARIO_METHOD(UnorderedSetTestsFixture, "loading unordered_set after saving", "[binary][container][unordered_set]")
{
    using TestedObject = std::unordered_set<int>;

    GIVEN("saved unordered_set")
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