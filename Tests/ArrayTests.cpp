#include <catch.hpp>

#include <Inscription/ArrayScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class ArrayTestsFixture : public BinaryFixture
{};

TEST_CASE_METHOD(ArrayTestsFixture, "std::array")
{
    using TestedObject = std::array<int, 5>;

    SECTION("save")
    {
        TestedObject saved;

        const auto startingGroup = dataGeneration.Generator<int>().RandomGroup(5);

        REQUIRE(saved.size() == startingGroup.size());

        for (size_t i = 0; i < startingGroup.size(); ++i)
            saved[i] = startingGroup[i];

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