#include <catch.hpp>

#include <Inscription/MultiSetScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"
#include "JsonFixture.h"

#include "TestClass.h"

class MultiSetBinaryTestsFixture : public BinaryFixture
{
public:
    using Integers = std::multiset<int>;
    using NestedIntegers = std::multiset<std::multiset<int>>;
    using Classes = std::multiset<TestClass>;
};

class MultiSetJsonTestsFixture : public JsonFixture
{
public:
    using Integers = std::multiset<int>;
    using NestedIntegers = std::multiset<std::multiset<int>>;
    using Classes = std::multiset<TestClass>;
};

SCENARIO_METHOD(MultiSetBinaryTestsFixture, "loading multiset after saving binary", "[binary][container][multiset]")
{
    GIVEN("saved empty multiset")
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

    GIVEN("saved single element multiset")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(1);

        for (auto& loop : startingGroup)
            saved.emplace(loop);

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

    GIVEN("saved multiset")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);

        for (auto& loop : startingGroup)
            saved.emplace(loop);

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

    GIVEN("saved nested multisets")
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

    GIVEN("saved multiset with objects")
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

SCENARIO_METHOD(MultiSetJsonTestsFixture, "loading multiset after saving json", "[json][container][multiset]")
{
    GIVEN("saved empty multiset")
    {
        Integers saved;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("empty_multiset", saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("empty_multiset", loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded.empty());
            }
        }
    }

    GIVEN("saved single element multiset")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(1);

        for (auto& loop : startingGroup)
            saved.emplace(loop);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("multiset", saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("multiset", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved multiset")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);

        for (auto& loop : startingGroup)
            saved.emplace(loop);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("multiset", saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("multiset", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved nested multisets")
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
            outputArchive("nested_multiset", saved);
        }

        WHEN("loading")
        {
            NestedIntegers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("nested_multiset", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved multiset with objects")
    {
        const auto objects = dataGeneration.RandomStackGroup<TestClass, int, std::string>(5);

        const Classes saved{ objects.begin(), objects.end() };

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("multiset", saved);
        }

        WHEN("loading")
        {
            Classes loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("multiset", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }
}