#include <catch.hpp>

#include <Inscription/StackScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class StackTestsFixture : public BinaryFixture
{};

SCENARIO_METHOD(StackTestsFixture, "loading stack after saving", "[binary][container][stack]")
{
    using TestedObject = std::stack<int>;

    GIVEN("saved stack")
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