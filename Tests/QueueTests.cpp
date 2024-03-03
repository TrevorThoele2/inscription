#include <catch.hpp>

#include <Inscription/QueueScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class QueueTestsFixture : public BinaryFixture
{};

TEST_CASE_METHOD(QueueTestsFixture, "std::queue")
{
    using TestedObject = std::queue<int>;

    SECTION("save")
    {
        TestedObject saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);

        for (auto& loop : startingGroup)
            saved.push(loop);

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