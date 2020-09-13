#include <catch.hpp>

#include <Inscription/SetScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"
#include "JsonFixture.h"

#include "TestClass.h"

class SetBinaryTestsFixture : public BinaryFixture
{
public:
    using Integers = std::set<int>;
    using NestedIntegers = std::set<std::set<int>>;
    using Classes = std::set<TestClass>;
};

class SetJsonTestsFixture : public JsonFixture
{
public:
    using Integers = std::set<int>;
    using NestedIntegers = std::set<std::set<int>>;
    using Classes = std::set<TestClass>;
};

SCENARIO_METHOD(SetBinaryTestsFixture, "loading set after saving binary", "[binary][container][set]")
{
    GIVEN("saved empty set")
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

    GIVEN("saved single element set")
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

    GIVEN("saved set")
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
            std::set<int> loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved nested sets")
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

    GIVEN("saved set with objects")
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

SCENARIO_METHOD(SetJsonTestsFixture, "loading set after saving json", "[json][container][set]")
{
    GIVEN("saved empty set")
    {
        Integers saved;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("empty_set", saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("empty_set", loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded.empty());
            }
        }
    }

    GIVEN("saved single element set")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(1);

        for (auto& loop : startingGroup)
            saved.emplace(loop);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("set", saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("set", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved set")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);

        for (auto& loop : startingGroup)
            saved.emplace(loop);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("set", saved);
        }

        WHEN("loading")
        {
            std::set<int> loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("set", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved nested sets")
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
            outputArchive("nested_set", saved);
        }

        WHEN("loading")
        {
            NestedIntegers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("nested_set", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved set with objects")
    {
        const auto objects = dataGeneration.RandomStackGroup<TestClass, int, std::string>(5);

        const Classes saved{ objects.begin(), objects.end() };

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("set", saved);
        }

        WHEN("loading")
        {
            Classes loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("set", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }
}