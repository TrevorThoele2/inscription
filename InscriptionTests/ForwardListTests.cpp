#include <boost/test/unit_test.hpp>

#include <Inscription/ForwardList.h>

#include "BinaryFixture.h"

class ForwardListTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(ForwardListTests, ForwardListTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    std::forward_list<int> forwardList;

    {
        const auto startingGroup = dataGeneration.Generator<int>().RandomGroup(5);

        for (auto& loop : startingGroup)
            forwardList.push_front(loop);
    }

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(forwardList);
    }

    std::forward_list<int> n_forwardList;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_forwardList);
    }

    BOOST_REQUIRE(!n_forwardList.empty());
    BOOST_REQUIRE(n_forwardList == forwardList);
}

BOOST_AUTO_TEST_SUITE_END()