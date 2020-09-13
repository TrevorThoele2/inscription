#include <catch.hpp>

#include <Inscription/MultimapScribe.h>
#include <Inscription/StringScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"
#include "JsonFixture.h"

#include "TestClass.h"

class MultiMapBinaryTestsFixture : public BinaryFixture
{
public:
    using IntegerToStrings = std::multimap<int, std::string>;
    using IntegerToClasses = std::multimap<int, TestClass>;
};

class MultiMapJsonTestsFixture : public JsonFixture
{
public:
    using IntegerToStrings = std::multimap<int, std::string>;
    using IntegerToClasses = std::multimap<int, TestClass>;
};

SCENARIO_METHOD(MultiMapBinaryTestsFixture, "loading multimap after saving binary", "[binary][container][multimap]")
{
    GIVEN("saved empty multimap")
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

    GIVEN("saved single element multimap")
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

    GIVEN("saved multimap")
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

    GIVEN("saved multimap with objects")
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

SCENARIO_METHOD(MultiMapJsonTestsFixture, "loading multimap after saving json", "[json][container][multimap]")
{
    GIVEN("saved empty multimap")
    {
        IntegerToStrings saved;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("empty_multimap", saved);
        }

        WHEN("loading")
        {
            IntegerToStrings loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("empty_multimap", loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded.empty());
            }
        }
    }

    GIVEN("saved single element multimap")
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
            outputArchive("multimap", saved);
        }

        WHEN("loading")
        {
            IntegerToStrings loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("multimap", loaded);

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
            outputArchive("multimap", saved);
        }

        WHEN("loading")
        {
            IntegerToStrings loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("multimap", loaded);

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
            inputArchive("multimap", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved multimap with objects")
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
            outputArchive("multimap", saved);
        }

        WHEN("loading")
        {
            IntegerToClasses loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("multimap", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }
}