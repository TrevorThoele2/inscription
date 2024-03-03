#include <boost/test/unit_test.hpp>

#include <Inscription/Stack.h>
#include <Inscription/Numeric.h>

#include "BinaryFixture.h"

class StackTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(StackTests, StackTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    using TestedObject = std::stack<int>;

    TestedObject saved;

    {
        const auto startingGroup = dataGeneration.Generator<int>().RandomGroup(5);

        for (auto& loop : startingGroup)
            saved.emplace(loop);

        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(saved);
    }

    {
        std::stack<int> loaded;

        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loaded);

        BOOST_REQUIRE(!loaded.empty());
        BOOST_REQUIRE(loaded == saved);
    }
}

BOOST_AUTO_TEST_SUITE_END()