#include <boost/test/unit_test.hpp>

#include <Inscription/ArrayScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class ArrayTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(ArrayTests, ArrayTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    using TestedObject = std::array<int, 5>;

    TestedObject saved;

    {
        const auto startingGroup = dataGeneration.Generator<int>().RandomGroup(5);

        BOOST_REQUIRE(saved.size() == startingGroup.size());

        for (size_t i = 0; i < startingGroup.size(); ++i)
            saved[i] = startingGroup[i];

        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(saved);
    }

    {
        TestedObject loaded;

        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loaded);

        BOOST_REQUIRE(!loaded.empty());
        BOOST_REQUIRE(loaded == saved);
    }
}

BOOST_AUTO_TEST_SUITE_END()