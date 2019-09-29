#include <catch.hpp>

#include <Inscription/VectorScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class VectorTestsFixture : public BinaryFixture
{};

SCENARIO_METHOD(VectorTestsFixture, "loading vector after saving", "[binary][container][vector]")
{
    using TestedObject = std::vector<int>;

    GIVEN("saved vector")
    {
        TestedObject saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);
        for (auto& loop : startingGroup)
            saved.push_back(loop);

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