#include <catch.hpp>

#include <Inscription/UnorderedMultisetScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class UnorderedMultiSetTestsFixture : public BinaryFixture
{};

TEST_CASE_METHOD(UnorderedMultiSetTestsFixture, "std::unordered_multiset")
{
    using TestedObject = std::unordered_multiset<int>;

    SECTION("save")
    {
        TestedObject saved;

        const auto startingGroup = dataGeneration.Generator<int>().RandomGroup(5);

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