#include <boost/test/unit_test.hpp>

#include <Inscription/QueueScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class QueueTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(QueueTests, QueueTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    using TestedObject = std::queue<int>;

    TestedObject saved;

    {
        const auto startingGroup = dataGeneration.Generator<int>().RandomGroup(5);

        for (auto& loop : startingGroup)
            saved.push(loop);

        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(saved);
    }

    {
        std::queue<int> loaded;

        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loaded);

        BOOST_REQUIRE(!loaded.empty());
        BOOST_REQUIRE(loaded == saved);
    }
}

BOOST_AUTO_TEST_SUITE_END()