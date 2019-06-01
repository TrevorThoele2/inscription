#include <boost/test/unit_test.hpp>

#include <Inscription/Queue.h>

#include "BinaryFixture.h"

class QueueTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(QueueTests, QueueTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    std::queue<int> queue;

    {
        const auto startingGroup = dataGeneration.Generator<int>().RandomGroup(5);

        for (auto& loop : startingGroup)
            queue.push(loop);
    }

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(queue);
    }

    std::queue<int> n_queue;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_queue);
    }

    BOOST_REQUIRE(!n_queue.empty());
    BOOST_REQUIRE(n_queue == queue);
}

BOOST_AUTO_TEST_SUITE_END()