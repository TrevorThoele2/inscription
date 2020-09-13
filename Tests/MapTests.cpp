#include <catch.hpp>

#include <Inscription/MapScribe.h>
#include <Inscription/StringScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"
#include "JsonFixture.h"

#include "TestClass.h"

class MapBinaryTestsFixture : public BinaryFixture
{
public:
    using IntegerToStrings = std::map<int, std::string>;
    using IntegerToClasses = std::map<int, TestClass>;
};

class MapJsonTestsFixture : public JsonFixture
{
public:
    using IntegerToStrings = std::map<int, std::string>;
    using IntegerToClasses = std::map<int, TestClass>;
};

SCENARIO_METHOD(MapBinaryTestsFixture, "loading map after saving binary", "[binary][container][map]")
{
    GIVEN("saved empty map")
    {
        IntegerToStrings saved;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
        {
            IntegerToStrings loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded.empty());
            }
        }
    }

    GIVEN("saved single element map")
    {
        IntegerToStrings saved;

        const auto startingGroupKeys = dataGeneration.RandomGroup<int>(1);
        const auto startingGroupValues = dataGeneration.RandomGroup<std::string>(1);

        for (size_t i = 0; i < startingGroupKeys.size(); ++i)
        {
            const auto key = startingGroupKeys[i];
            const auto value = startingGroupValues[i];

            saved.emplace(key, value);
        }

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
        {
            IntegerToStrings loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved map")
    {
        IntegerToStrings saved;

        const auto startingGroupKeys = dataGeneration.RandomGroup<int>(5);
        const auto startingGroupValues = dataGeneration.RandomGroup<std::string>(5);

        for (size_t i = 0; i < startingGroupKeys.size(); ++i)
        {
            const auto key = startingGroupKeys[i];
            const auto value = startingGroupValues[i];

            saved.emplace(key, value);
        }

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
        {
            IntegerToStrings loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved map with objects")
    {
        IntegerToClasses saved;

        const auto startingGroupKeys = dataGeneration.RandomGroup<int>(5);
        const auto startingGroupValues = dataGeneration.RandomStackGroup<TestClass, int, std::string>(5);

        for (size_t i = 0; i < startingGroupKeys.size(); ++i)
        {
            const auto key = startingGroupKeys[i];
            const auto value = startingGroupValues[i];

            saved.emplace(key, value);
        }

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
        {
            IntegerToClasses loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }
}

SCENARIO_METHOD(MapJsonTestsFixture, "loading map after saving json", "[json][container][map]")
{
    GIVEN("saved empty map")
    {
        IntegerToStrings saved;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("empty_map", saved);
        }

        WHEN("loading")
        {
            IntegerToStrings loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("empty_map", loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded.empty());
            }
        }
    }

    GIVEN("saved single element map")
    {
        IntegerToStrings saved;

        const auto startingGroupKeys = dataGeneration.RandomGroup<int>(1);
        const auto startingGroupValues = dataGeneration.RandomGroup<std::string>(1);

        for (size_t i = 0; i < startingGroupKeys.size(); ++i)
        {
            const auto key = startingGroupKeys[i];
            const auto value = startingGroupValues[i];

            saved.emplace(key, value);
        }

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("map", saved);
        }

        WHEN("loading")
        {
            IntegerToStrings loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("map", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved map")
    {
        IntegerToStrings saved;

        const auto startingGroupKeys = dataGeneration.RandomGroup<int>(5);
        const auto startingGroupValues = dataGeneration.RandomGroup<std::string>(5);

        for (size_t i = 0; i < startingGroupKeys.size(); ++i)
        {
            const auto key = startingGroupKeys[i];
            const auto value = startingGroupValues[i];

            saved.emplace(key, value);
        }

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("map", saved);
        }

        WHEN("loading")
        {
            IntegerToStrings loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("map", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved map with objects")
    {
        IntegerToClasses saved;

        const auto startingGroupKeys = dataGeneration.RandomGroup<int>(5);
        const auto startingGroupValues = dataGeneration.RandomStackGroup<TestClass, int, std::string>(5);

        for (size_t i = 0; i < startingGroupKeys.size(); ++i)
        {
            const auto key = startingGroupKeys[i];
            const auto value = startingGroupValues[i];

            saved.emplace(key, value);
        }

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("map", saved);
        }

        WHEN("loading")
        {
            IntegerToClasses loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("map", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }
}