#include <boost/test/unit_test.hpp>

#include <Inscription/List.h>

#include "BinaryFixture.h"

class ListTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(ListTests, ListTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    std::list<int> list;

    {
        const auto startingGroup = dataGeneration.Generator<int>().RandomGroup(5);

        for (auto& loop : startingGroup)
            list.push_front(loop);
    }

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(list);
    }

    std::list<int> n_list;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_list);
    }

    BOOST_REQUIRE(!n_list.empty());
    BOOST_REQUIRE(n_list == list);
}

BOOST_AUTO_TEST_SUITE_END()