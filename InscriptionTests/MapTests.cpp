#include <boost/test/unit_test.hpp>

#include <Inscription/Map.h>

#include "BinaryFixture.h"

class MapTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(MapTests, MapTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    std::map<int, std::string> map;

    {
        const auto startingGroupKeys = dataGeneration.Generator<int>().RandomGroup(5);
        const auto startingGroupValues = dataGeneration.Generator<std::string>().RandomGroup(5);

        for (size_t i = 0; i < startingGroupKeys.size(); ++i)
        {
            const auto key = startingGroupKeys[i];
            const auto value = startingGroupValues[i];

            map.emplace(key, value);
        }
    }

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(map);
    }

    std::map<int, std::string> n_map;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_map);
    }

    BOOST_REQUIRE(!n_map.empty());
    BOOST_REQUIRE(n_map == map);
}

BOOST_AUTO_TEST_SUITE_END()