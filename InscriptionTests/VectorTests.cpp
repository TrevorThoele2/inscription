#include <boost/test/unit_test.hpp>

#include <Inscription/Vector.h>

#include "BinaryFixture.h"

class VectorTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(VectorTests, VectorTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    std::vector<int> vector;

    {
        const auto startingGroup = dataGeneration.Generator<int>().RandomGroup(5);
        for (auto& loop : startingGroup)
            vector.push_back(loop);
    }

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(vector);
    }

    std::vector<int> n_vector;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_vector);
    }

    BOOST_REQUIRE(!n_vector.empty());
    BOOST_REQUIRE(n_vector == vector);
}

BOOST_AUTO_TEST_SUITE_END()