#include <catch.hpp>

#include <Inscription/UnorderedMapScribe.h>
#include <Inscription/StringScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"
#include "JsonFixture.h"

#include "TestClass.h"

class UnorderedMapBinaryTestsFixture : public BinaryFixture
{
public:
    using IntegerToStrings = std::unordered_map<int, std::string>;
    using IntegerToClasses = std::unordered_map<int, TestClass>;
};

class UnorderedMapJsonTestsFixture : public JsonFixture
{
public:
    using IntegerToStrings = std::unordered_map<int, std::string>;
    using IntegerToClasses = std::unordered_map<int, TestClass>;
};

SCENARIO_METHOD(UnorderedMapBinaryTestsFixture, "loading unordered_map after saving binary", "[binary][container][unordered_map]")
{
    GIVEN("saved empty unordered_map")
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

    GIVEN("saved single element unordered_map")
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

    GIVEN("saved unordered_map")
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

        WHEN("loading over occupied container")
        {
            IntegerToStrings loaded;

            const auto occupiedGroupKeys = dataGeneration.RandomGroup<int>(5);
            const auto occupiedGroupValues = dataGeneration.RandomGroup<std::string>(5);

            for (size_t i = 0; i < occupiedGroupKeys.size(); ++i)
            {
                const auto key = occupiedGroupKeys[i];
                const auto value = occupiedGroupValues[i];

                loaded.emplace(key, value);
            }

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved unordered_map with objects")
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

SCENARIO_METHOD(UnorderedMapJsonTestsFixture, "loading unordered_map after saving json", "[json][container][unordered_map]")
{
    GIVEN("saved empty unordered_map")
    {
        IntegerToStrings saved;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("empty_unordered_map", saved);
        }

        WHEN("loading")
        {
            IntegerToStrings loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("empty_unordered_map", loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded.empty());
            }
        }
    }

    GIVEN("saved single element unordered_map")
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
            outputArchive("unordered_map", saved);
        }

        WHEN("loading")
        {
            IntegerToStrings loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("unordered_map", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved unordered_map")
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
            outputArchive("unordered_map", saved);
        }

        WHEN("loading")
        {
            IntegerToStrings loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("unordered_map", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }

        WHEN("loading over occupied container")
        {
            IntegerToStrings loaded;

            const auto occupiedGroupKeys = dataGeneration.RandomGroup<int>(5);
            const auto occupiedGroupValues = dataGeneration.RandomGroup<std::string>(5);

            for (size_t i = 0; i < occupiedGroupKeys.size(); ++i)
            {
                const auto key = occupiedGroupKeys[i];
                const auto value = occupiedGroupValues[i];

                loaded.emplace(key, value);
            }

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("unordered_map", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved unordered_map with objects")
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
            outputArchive("unordered_map", saved);
        }

        WHEN("loading")
        {
            IntegerToClasses loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("unordered_map", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }
}