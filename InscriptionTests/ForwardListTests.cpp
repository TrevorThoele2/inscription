#include <boost/test/unit_test.hpp>

#include <Inscription/ForwardListScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"

class ForwardListTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(ForwardListTests, ForwardListTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    using TestedObject = std::forward_list<int>;

    TestedObject saved;

    {
        const auto startingGroup = dataGeneration.Generator<int>().RandomGroup(5);

        for (auto& loop : startingGroup)
            saved.push_front(loop);

        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(saved);
    }

    {
        TestedObject loaded;

        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loaded);

        BOOST_REQUIRE(!loaded.empty());
        BOOST_REQUIRE(loaded == saved);
    }
}

BOOST_AUTO_TEST_SUITE_END()