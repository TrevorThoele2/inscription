#include <catch.hpp>

#include <Inscription/SetScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class SetTestsFixture : public BinaryFixture
{};

TEST_CASE_METHOD(SetTestsFixture, "std::set")
{
    using TestedObject = std::set<int>;

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
            std::set<int> loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            REQUIRE(!loaded.empty());
            REQUIRE(loaded == saved);
        }
    }
}