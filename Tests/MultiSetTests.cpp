#include <catch.hpp>

#include <Inscription/MultiSetScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class MultiSetTestsFixture : public BinaryFixture
{};

TEST_CASE_METHOD(MultiSetTestsFixture, "std::multiset")
{
    using TestedObject = std::multiset<int>;

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