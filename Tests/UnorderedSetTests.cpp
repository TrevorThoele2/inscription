#include <catch.hpp>

#include <Inscription/UnorderedSetScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class UnorderedSetTestsFixture : public BinaryFixture
{};

TEST_CASE_METHOD(UnorderedSetTestsFixture, "std::unordered_set")
{
    using TestedObject = std::unordered_set<int>;

    SECTION("save")
    {
        TestedObject saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);

        for (auto& loop : startingGroup)
            saved.emplace(loop);

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