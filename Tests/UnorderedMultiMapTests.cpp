#include <catch.hpp>

#include <Inscription/UnorderedMultimapScribe.h>
#include <Inscription/StringScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"
#include "JsonFixture.h"

#include "TestClass.h"

class UnorderedMultiMapBinaryTestsFixture : public BinaryFixture
{
public:
    using IntegerToStrings = std::unordered_multimap<int, std::string>;
    using IntegerToClasses = std::unordered_multimap<int, TestClass>;
};

class UnorderedMultiMapJsonTestsFixture : public JsonFixture
{
public:
    using IntegerToStrings = std::unordered_multimap<int, std::string>;
    using IntegerToClasses = std::unordered_multimap<int, TestClass>;
};

SCENARIO_METHOD(
    UnorderedMultiMapBinaryTestsFixture,
    "loading unordered_multimap after saving binary",
    "[binary][container][unordered_multimap]"
) {
    GIVEN("saved empty unordered_multimap")
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

    GIVEN("saved single element unordered_multimap")
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

    GIVEN("saved unordered_multimap")
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

    GIVEN("saved unordered_multimap with objects")
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

SCENARIO_METHOD(UnorderedMultiMapJsonTestsFixture, "loading unordered_multimap after saving json", "[json][container][unordered_multimap]")
{
    GIVEN("saved empty unordered_multimap")
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

    GIVEN("saved single element unordered_multimap")
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
            outputArchive("unordered_multimap", saved);
        }

        WHEN("loading")
        {
            IntegerToStrings loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("unordered_multimap", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved unordered_multimap")
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
            outputArchive("unordered_multimap", saved);
        }

        WHEN("loading")
        {
            IntegerToStrings loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("unordered_multimap", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved unordered_multimap with objects")
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
            outputArchive("unordered_multimap", saved);
        }

        WHEN("loading")
        {
            IntegerToClasses loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("unordered_multimap", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }
}