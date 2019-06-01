#include <boost/test/unit_test.hpp>

#include <Inscription/MultiSet.h>

#include "BinaryFixture.h"

class MultiSetTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(MultiSetTests, MultiSetTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    std::multiset<int> multiSet;

    {
        const auto startingGroup = dataGeneration.Generator<int>().RandomGroup(5);

        for (auto& loop : startingGroup)
            multiSet.emplace(loop);
    }

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(multiSet);
    }

    std::multiset<int> n_multiSet;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_multiSet);
    }

    BOOST_REQUIRE(!n_multiSet.empty());
    BOOST_REQUIRE(n_multiSet == multiSet);
}

BOOST_AUTO_TEST_SUITE_END()