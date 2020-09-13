#include <catch.hpp>

#include <Inscription/ForwardListScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"
#include "JsonFixture.h"

#include "TestClass.h"

class ForwardListBinaryTestsFixture : public BinaryFixture
{
public:
    using Integers = std::forward_list<int>;
    using NestedIntegers = std::forward_list<std::forward_list<int>>;
    using Classes = std::forward_list<TestClass>;
};

class ForwardListJsonTestsFixture : public JsonFixture
{
public:
    using Integers = std::forward_list<int>;
    using NestedIntegers = std::forward_list<std::forward_list<int>>;
    using Classes = std::forward_list<TestClass>;
};

SCENARIO_METHOD(ForwardListBinaryTestsFixture, "loading forward_list after saving binary", "[binary][container][forward_list]")
{
    GIVEN("saved empty forward_list")
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

    GIVEN("saved single element forward_list")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(1);

        for (auto& loop : startingGroup)
            saved.push_front(loop);

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

    GIVEN("saved forward_list")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);

        for (auto& loop : startingGroup)
            saved.push_front(loop);

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

    GIVEN("saved nested forward_lists")
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

    GIVEN("saved forward_list with objects")
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

SCENARIO_METHOD(ForwardListJsonTestsFixture, "loading forward_list after saving json", "[json][container][forward_list]")
{
    GIVEN("saved empty forward_list")
    {
        Integers saved;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("empty_forward_list", saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("empty_forward_list", loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded.empty());
            }
        }
    }

    GIVEN("saved single element forward_list")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(1);

        for (auto& loop : startingGroup)
            saved.push_front(loop);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("forward_list", saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("forward_list", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved forward_list")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);

        for (auto& loop : startingGroup)
            saved.push_front(loop);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("forward_list", saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("forward_list", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved nested forward_lists")
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
            outputArchive("nested_forward_list", saved);
        }

        WHEN("loading")
        {
            NestedIntegers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("nested_forward_list", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved forward_list with objects")
    {
        const auto objects = dataGeneration.RandomStackGroup<TestClass, int, std::string>(5);

        const Classes saved{ objects.begin(), objects.end() };

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("forward_list", saved);
        }

        WHEN("loading")
        {
            Classes loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("forward_list", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }
}