#include <boost/test/unit_test.hpp>

#include <Inscription/MultiMap.h>

#include "BinaryFixture.h"

class MultiMapTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(MultiMapTests, MultiMapTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    std::multimap<int, std::string> multiMap;

    {
        const auto startingGroupKeys = dataGeneration.Generator<int>().RandomGroup(5);
        const auto startingGroupValues = dataGeneration.Generator<std::string>().RandomGroup(5);

        for (size_t i = 0; i < startingGroupKeys.size(); ++i)
        {
            const auto key = startingGroupKeys[i];
            const auto value = startingGroupValues[i];

            multiMap.emplace(key, value);
        }
    }

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(multiMap);
    }

    std::multimap<int, std::string> n_multiMap;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_multiMap);
    }

    BOOST_REQUIRE(!n_multiMap.empty());
    BOOST_REQUIRE(n_multiMap == multiMap);
}

BOOST_AUTO_TEST_SUITE_END()