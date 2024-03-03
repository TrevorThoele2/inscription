#include <boost/test/unit_test.hpp>

#include <Inscription/Array.h>

#include "BinaryFixture.h"

class ArrayTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(ArrayTests, ArrayTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    std::array<int, 5> array;

    {
        const auto startingGroup = dataGeneration.Generator<int>().RandomGroup(5);

        BOOST_REQUIRE(array.size() == startingGroup.size());

        for (size_t i = 0; i < startingGroup.size(); ++i)
            array[i] = startingGroup[i];
    }

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(array);
    }

    std::array<int, 5> n_array;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_array);
    }

    BOOST_REQUIRE(!n_array.empty());
    BOOST_REQUIRE(n_array == array);
}

BOOST_AUTO_TEST_SUITE_END()