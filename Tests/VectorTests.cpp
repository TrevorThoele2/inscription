#include <catch.hpp>

#include <Inscription/VectorScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class VectorTestsFixture : public BinaryFixture
{};

TEST_CASE_METHOD(VectorTestsFixture, "std::vector")
{
    using TestedObject = std::vector<int>;

    SECTION("save")
    {
        TestedObject saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);
        for (auto& loop : startingGroup)
            saved.push_back(loop);

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