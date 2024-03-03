#include <boost/test/unit_test.hpp>

#include <Inscription/VectorScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class VectorTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(VectorTests, VectorTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    using TestedObject = std::vector<int>;

    TestedObject saved;

    {
        const auto startingGroup = dataGeneration.Generator<int>().RandomGroup(5);
        for (auto& loop : startingGroup)
            saved.push_back(loop);

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