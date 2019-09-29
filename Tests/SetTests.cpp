#include <catch.hpp>

#include <Inscription/SetScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class SetTestsFixture : public BinaryFixture
{};

SCENARIO_METHOD(SetTestsFixture, "loading set after saving", "[binary][container][set]")
{
    using TestedObject = std::set<int>;

    GIVEN("saved set")
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
            std::set<int> loaded;

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