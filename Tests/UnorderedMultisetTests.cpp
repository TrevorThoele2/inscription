#include <catch.hpp>

#include <Inscription/UnorderedMultisetScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class UnorderedMultiSetTestsFixture : public BinaryFixture
{};

SCENARIO_METHOD(
    UnorderedMultiSetTestsFixture,
    "loading unordered_multiset after saving", "[binary][container][unordered_multiset]"
) {
    using TestedObject = std::unordered_multiset<int>;

    GIVEN("saved unordered_multiset")
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