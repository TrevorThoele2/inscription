#include <boost/test/unit_test.hpp>

#include <Inscription/UnorderedMultiSet.h>

#include "BinaryFixture.h"

class UnorderedMultiSetTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(UnorderedMultiSetTests, UnorderedMultiSetTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    std::unordered_multiset<int> unorderedMultiSet;

    {
        const auto startingGroup = dataGeneration.Generator<int>().RandomGroup(5);

        for (auto& loop : startingGroup)
            unorderedMultiSet.emplace(loop);
    }

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(unorderedMultiSet);
    }

    std::unordered_multiset<int> n_unorderedMultiSet;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_unorderedMultiSet);
    }

    BOOST_REQUIRE(!n_unorderedMultiSet.empty());
    BOOST_REQUIRE(n_unorderedMultiSet == unorderedMultiSet);
}

BOOST_AUTO_TEST_SUITE_END()