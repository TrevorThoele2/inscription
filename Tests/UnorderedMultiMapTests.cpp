#include <catch.hpp>

#include <Inscription/UnorderedMultimapScribe.h>
#include <Inscription/StringScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class UnorderedMultiMapTestsFixture : public BinaryFixture
{};

SCENARIO_METHOD(
    UnorderedMultiMapTestsFixture,
    "loading unordered_multimap after saving",
    "[binary][container][unordered_multimap]"
) {
    using TestedObject = std::unordered_multimap<int, std::string>;

    GIVEN("saved unordered_multimap")
    {
        TestedObject saved;

        const auto startingGroupKeys = dataGeneration.RandomGroup<int>(5);
        const auto startingGroupValues = dataGeneration.RandomGroup<std::string>(5);

        for (size_t i = 0; i < startingGroupKeys.size(); ++i)
        {
            const auto key = startingGroupKeys[i];
            const auto value = startingGroupValues[i];

            saved.emplace(key, value);
        }

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