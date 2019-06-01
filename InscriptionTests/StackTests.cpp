#include <boost/test/unit_test.hpp>

#include <Inscription/Stack.h>

#include "BinaryFixture.h"

class StackTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(StackTests, StackTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    std::stack<int> stack;

    {
        const auto startingGroup = dataGeneration.Generator<int>().RandomGroup(5);

        for (auto& loop : startingGroup)
            stack.emplace(loop);
    }

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(stack);
    }

    std::stack<int> n_stack;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_stack);
    }

    BOOST_REQUIRE(!n_stack.empty());
    BOOST_REQUIRE(n_stack == stack);
}

BOOST_AUTO_TEST_SUITE_END()