#include <catch.hpp>

#include <Inscription/StackScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class StackTestsFixture : public BinaryFixture
{};

TEST_CASE_METHOD(StackTestsFixture, "std::stack")
{
    using TestedObject = std::stack<int>;

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