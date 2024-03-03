#include <boost/test/unit_test.hpp>

#include <Inscription/UnorderedMap.h>

#include "BinaryFixture.h"

class UnorderedMapTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(UnorderedMapTests, UnorderedMapTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    std::unordered_map<int, std::string> unorderedMap;

    {
        const auto startingGroupKeys = dataGeneration.Generator<int>().RandomGroup(5);
        const auto startingGroupValues = dataGeneration.Generator<std::string>().RandomGroup(5);

        for (size_t i = 0; i < startingGroupKeys.size(); ++i)
        {
            const auto key = startingGroupKeys[i];
            const auto value = startingGroupValues[i];

            unorderedMap.emplace(key, value);
        }
    }

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(unorderedMap);
    }

    std::unordered_map<int, std::string> n_unorderedMap;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_unorderedMap);
    }

    BOOST_REQUIRE(!n_unorderedMap.empty());
    BOOST_REQUIRE(n_unorderedMap == unorderedMap);
}

BOOST_AUTO_TEST_SUITE_END()