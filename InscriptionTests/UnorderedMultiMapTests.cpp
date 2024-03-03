#include <boost/test/unit_test.hpp>

#include <Inscription/UnorderedMultiMap.h>

#include "BinaryFixture.h"

class UnorderedMultiMapTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(UnorderedMultiMapTests, UnorderedMultiMapTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    std::unordered_multimap<int, std::string> unorderedMultiMap;

    {
        const auto startingGroupKeys = dataGeneration.Generator<int>().RandomGroup(5);
        const auto startingGroupValues = dataGeneration.Generator<std::string>().RandomGroup(5);

        for (size_t i = 0; i < startingGroupKeys.size(); ++i)
        {
            const auto key = startingGroupKeys[i];
            const auto value = startingGroupValues[i];

            unorderedMultiMap.emplace(key, value);
        }
    }

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(unorderedMultiMap);
    }

    std::unordered_multimap<int, std::string> n_unorderedMultiMap;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_unorderedMultiMap);
    }

    BOOST_REQUIRE(!n_unorderedMultiMap.empty());
    BOOST_REQUIRE(n_unorderedMultiMap == unorderedMultiMap);
}

BOOST_AUTO_TEST_SUITE_END()