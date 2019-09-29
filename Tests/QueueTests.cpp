#include <catch.hpp>

#include <Inscription/QueueScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class QueueTestsFixture : public BinaryFixture
{};

SCENARIO_METHOD(QueueTestsFixture, "loading queue after saving", "[binary][container][queue]")
{
    using TestedObject = std::queue<int>;

    GIVEN("saved queue")
    {
        TestedObject saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);

        for (auto& loop : startingGroup)
            saved.push(loop);

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