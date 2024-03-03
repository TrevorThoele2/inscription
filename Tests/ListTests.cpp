#include <catch.hpp>

#include <Inscription/ListScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class ListTestsFixture : public BinaryFixture
{};

SCENARIO_METHOD(ListTestsFixture, "loading list after saving", "[binary][container][list]")
{
    using TestedObject = std::list<int>;

    GIVEN("saved list")
    {
        TestedObject saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);

        for (auto& loop : startingGroup)
            saved.push_front(loop);

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