#include <catch.hpp>

#include <Inscription/ListScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class ListTestsFixture : public BinaryFixture
{};

TEST_CASE_METHOD(ListTestsFixture, "std::list")
{
    using TestedObject = std::list<int>;

    SECTION("save")
    {
        TestedObject saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);

        for (auto& loop : startingGroup)
            saved.push_front(loop);

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