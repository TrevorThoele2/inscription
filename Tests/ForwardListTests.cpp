#include <catch.hpp>

#include <Inscription/ForwardListScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class ForwardListTestsFixture : public BinaryFixture
{};

SCENARIO_METHOD(ForwardListTestsFixture, "loading forward_list after saving", "[binary][container][forward_list]")
{
    using TestedObject = std::forward_list<int>;

    GIVEN("saved forward_list")
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