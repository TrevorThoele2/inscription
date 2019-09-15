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

        const auto startingGroup = dataGeneration.RandomGroup<int>(saved.size());

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