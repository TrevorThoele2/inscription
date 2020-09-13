#include <catch.hpp>

#include <Inscription/VectorScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"
#include "JsonFixture.h"

#include "TestClass.h"

class VectorBinaryTestsFixture : public BinaryFixture
{
public:
    using Integers = std::vector<int>;
    using NestedIntegers = std::vector<std::vector<int>>;
    using Classes = std::vector<TestClass>;
};

class VectorJsonTestsFixture : public JsonFixture
{
public:
    using Integers = std::vector<int>;
    using NestedIntegers = std::vector<std::vector<int>>;
    using Classes = std::vector<TestClass>;
};

SCENARIO_METHOD(VectorBinaryTestsFixture, "loading vector after saving binary", "[binary][container][vector]")
{
    GIVEN("saved empty vector")
    {
        Integers saved;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded.empty());
            }
        }
    }

    GIVEN("saved single element vector")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(1);

        for (auto& loop : startingGroup)
            saved.push_back(loop);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved vector")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);
        for (auto& loop : startingGroup)
            saved.push_back(loop);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved nested vectors")
    {
        const auto startingGroup0 = dataGeneration.RandomGroup<int>(5);
        const auto startingGroup1 = dataGeneration.RandomGroup<int>(5);
        const auto startingGroup2 = dataGeneration.RandomGroup<int>(5);
        const auto startingGroup3 = dataGeneration.RandomGroup<int>(5);
        const auto startingGroup4 = dataGeneration.RandomGroup<int>(5);

        const NestedIntegers saved
        {
            Integers(startingGroup0.begin(), startingGroup0.end()),
            Integers(startingGroup1.begin(), startingGroup1.end()),
            Integers(startingGroup2.begin(), startingGroup2.end()),
            Integers(startingGroup3.begin(), startingGroup3.end()),
            Integers(startingGroup4.begin(), startingGroup4.end())
        };

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
        {
            NestedIntegers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved vector with objects")
    {
        const auto objects = dataGeneration.RandomStackGroup<TestClass, int, std::string>(5);

        const Classes saved{ objects.begin(), objects.end() };

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
        {
            Classes loaded;

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

SCENARIO_METHOD(VectorJsonTestsFixture, "loading vector after saving json", "[json][container][vector]")
{
    GIVEN("saved empty vector")
    {
        Integers saved;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("vector", saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("vector", loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded.empty());
            }
        }
    }

    GIVEN("saved single element vector")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(1);

        for (auto& loop : startingGroup)
            saved.push_back(loop);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("vector", saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("vector", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved vector")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);
        for (auto& loop : startingGroup)
            saved.push_back(loop);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("vector", saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("vector", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved nested vectors")
    {
        const auto startingGroup0 = dataGeneration.RandomGroup<int>(5);
        const auto startingGroup1 = dataGeneration.RandomGroup<int>(5);
        const auto startingGroup2 = dataGeneration.RandomGroup<int>(5);
        const auto startingGroup3 = dataGeneration.RandomGroup<int>(5);
        const auto startingGroup4 = dataGeneration.RandomGroup<int>(5);

        const NestedIntegers saved
        {
            Integers(startingGroup0.begin(), startingGroup0.end()),
            Integers(startingGroup1.begin(), startingGroup1.end()),
            Integers(startingGroup2.begin(), startingGroup2.end()),
            Integers(startingGroup3.begin(), startingGroup3.end()),
            Integers(startingGroup4.begin(), startingGroup4.end())
        };

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("nested_vector", saved);
        }

        WHEN("loading")
        {
            NestedIntegers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("nested_vector", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved vector with objects")
    {
        const auto objects = dataGeneration.RandomStackGroup<TestClass, int, std::string>(5);

        const Classes saved{ objects.begin(), objects.end() };

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("vector", saved);
        }

        WHEN("loading")
        {
            Classes loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("vector", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }
}