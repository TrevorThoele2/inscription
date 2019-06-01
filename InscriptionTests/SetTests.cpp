#include <boost/test/unit_test.hpp>

#include <Inscription/Set.h>

#include "BinaryFixture.h"

class SetTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(SetTests, SetTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    std::set<int> set;

    {
        const auto startingGroup = dataGeneration.Generator<int>().RandomGroup(5);

        for (auto& loop : startingGroup)
            set.emplace(loop);
    }

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(set);
    }

    std::set<int> n_set;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_set);
    }

    BOOST_REQUIRE(!n_set.empty());
    BOOST_REQUIRE(n_set == set);
}

BOOST_AUTO_TEST_SUITE_END()