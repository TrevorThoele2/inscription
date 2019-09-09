#include <catch.hpp>

#include <Inscription/UnorderedMultimapScribe.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"

class UnorderedMultiMapTestsFixture : public BinaryFixture
{};

TEST_CASE_METHOD(UnorderedMultiMapTestsFixture, "std::unordered_multimap")
{
    using TestedObject = std::unordered_multimap<int, std::string>;

    SECTION("save")
    {
        TestedObject saved;

        const auto startingGroupKeys = dataGeneration.Generator<int>().RandomGroup(5);
        const auto startingGroupValues = dataGeneration.Generator<std::string>().RandomGroup(5);

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

        SECTION("load")
        {
            TestedObject loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            REQUIRE(!loaded.empty());
            REQUIRE(loaded == saved);
        }
    }
}