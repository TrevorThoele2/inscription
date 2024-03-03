#include <catch.hpp>

#include <Inscription/UnorderedMapScribe.h>
#include <Inscription/StringScribe.h>
#include <Inscription/NumericScribe.h>

#include "GeneralFixture.h"

#include "TestClass.h"

class UnorderedMapTestsFixture : public GeneralFixture
{
public:
    using IntegerToStrings = std::unordered_map<int, std::string>;
    using IntegerToClasses = std::unordered_map<int, TestClass>;
};

SCENARIO_METHOD(UnorderedMapTestsFixture, "loading unordered_map after saving binary", "[binary][container][unordered_map]")
{
    GIVEN("saved empty unordered_map")
    {
        IntegerToStrings saved;

        Inscription::Binary::ToFile(saved, "Test.dat");

        WHEN("loading")
        {
            IntegerToStrings loaded;

            Inscription::Binary::FromFile(loaded, "Test.dat");

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

        Inscription::Binary::ToFile(saved, "Test.dat");

        WHEN("loading")
        {
            IntegerToStrings loaded;

            Inscription::Binary::FromFile(loaded, "Test.dat");

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

        Inscription::Binary::ToFile(saved, "Test.dat");

        WHEN("loading")
        {
            IntegerToStrings loaded;

            Inscription::Binary::FromFile(loaded, "Test.dat");

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

            Inscription::Binary::FromFile(loaded, "Test.dat");

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

        Inscription::Binary::ToFile(saved, "Test.dat");

        WHEN("loading")
        {
            IntegerToClasses loaded;

            Inscription::Binary::FromFile(loaded, "Test.dat");

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }
}

SCENARIO_METHOD(UnorderedMapTestsFixture, "loading unordered_map after saving json", "[json][container][unordered_map]")
{
    GIVEN("saved empty unordered_map")
    {
        IntegerToStrings saved;

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading")
        {
            IntegerToStrings loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

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

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading")
        {
            IntegerToStrings loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

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

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading")
        {
            IntegerToStrings loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

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

            Inscription::Json::FromFile(loaded, "Test.json");

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

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading")
        {
            IntegerToClasses loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }
}