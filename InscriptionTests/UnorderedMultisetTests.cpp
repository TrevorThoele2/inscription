#include <boost/test/unit_test.hpp>

#include <Inscription/UnorderedMultiSet.h>
#include <Inscription/Numeric.h>

#include "BinaryFixture.h"

class UnorderedMultiSetTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(UnorderedMultiSetTests, UnorderedMultiSetTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    using TestedObject = std::unordered_multiset<int>;

    TestedObject saved;

    {
        const auto startingGroup = dataGeneration.Generator<int>().RandomGroup(5);

        for (auto& loop : startingGroup)
            saved.emplace(loop);

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