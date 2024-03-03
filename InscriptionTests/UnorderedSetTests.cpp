#include <boost/test/unit_test.hpp>

#include <Inscription/UnorderedSet.h>

#include "BinaryFixture.h"

class UnorderedSetTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(UnorderedSetTests, UnorderedSetTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    std::unordered_set<int> unorderedSet;

    {
        const auto startingGroup = dataGeneration.Generator<int>().RandomGroup(5);

        for (auto& loop : startingGroup)
            unorderedSet.emplace(loop);
    }

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(unorderedSet);
    }

    std::unordered_set<int> n_unorderedSet;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_unorderedSet);
    }

    BOOST_REQUIRE(!n_unorderedSet.empty());
    BOOST_REQUIRE(n_unorderedSet == unorderedSet);
}

BOOST_AUTO_TEST_SUITE_END()