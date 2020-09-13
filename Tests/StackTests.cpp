#include <catch.hpp>

#include <Inscription/StackScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"
#include "JsonFixture.h"

#include "TestClass.h"

class StackBinaryTestsFixture : public BinaryFixture
{
public:
    using Integers = std::stack<int>;
    using NestedIntegers = std::stack<std::stack<int>>;
    using Classes = std::stack<TestClass>;
};

class StackJsonTestsFixture : public JsonFixture
{
public:
    using Integers = std::stack<int>;
    using NestedIntegers = std::stack<std::stack<int>>;
    using Classes = std::stack<TestClass>;
};

SCENARIO_METHOD(StackBinaryTestsFixture, "loading stack after saving binary", "[binary][container][stack]")
{
    GIVEN("saved empty stack")
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

    GIVEN("saved single element stack")
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

    GIVEN("saved stack")
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

        WHEN("loading over occupied container")
        {
            Integers loaded;

            const auto occupiedGroup = dataGeneration.RandomGroup<int>(5);

            for (auto& loop : occupiedGroup)
                loaded.emplace(loop);

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved nested stacks")
    {
        const auto constructGroup = [this]()
        {
            const auto group = dataGeneration.RandomGroup<int>(5);
            Integers stack;
            for (auto& i : group)
                stack.push(i);
            return stack;
        };

        const auto startingGroup0 = constructGroup();
        const auto startingGroup1 = constructGroup();
        const auto startingGroup2 = constructGroup();
        const auto startingGroup3 = constructGroup();
        const auto startingGroup4 = constructGroup();

        NestedIntegers saved;
        saved.push(startingGroup0);
        saved.push(startingGroup1);
        saved.push(startingGroup2);
        saved.push(startingGroup3);
        saved.push(startingGroup4);

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

    GIVEN("saved stack with objects")
    {
        const auto objects = dataGeneration.RandomStackGroup<TestClass, int, std::string>(5);

        Classes saved;
        saved.push(objects[0]);
        saved.push(objects[1]);
        saved.push(objects[2]);
        saved.push(objects[3]);
        saved.push(objects[4]);

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

SCENARIO_METHOD(StackJsonTestsFixture, "loading stack after saving json", "[json][container][stack]")
{
    GIVEN("saved empty stack")
    {
        Integers saved;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("empty_stack", saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("empty_stack", loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded.empty());
            }
        }
    }

    GIVEN("saved single element stack")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(1);

        for (auto& loop : startingGroup)
            saved.emplace(loop);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("stack", saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("stack", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved stack")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);

        for (auto& loop : startingGroup)
            saved.emplace(loop);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("stack", saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("stack", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }

        WHEN("loading over occupied container")
        {
            Integers loaded;

            const auto occupiedGroup = dataGeneration.RandomGroup<int>(5);

            for (auto& loop : occupiedGroup)
                loaded.emplace(loop);

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("stack", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved nested stacks")
    {
        const auto constructGroup = [this]()
        {
            const auto group = dataGeneration.RandomGroup<int>(5);
            Integers stack;
            for (auto& i : group)
                stack.push(i);
            return stack;
        };

        const auto startingGroup0 = constructGroup();
        const auto startingGroup1 = constructGroup();
        const auto startingGroup2 = constructGroup();
        const auto startingGroup3 = constructGroup();
        const auto startingGroup4 = constructGroup();

        NestedIntegers saved;
        saved.push(startingGroup0);
        saved.push(startingGroup1);
        saved.push(startingGroup2);
        saved.push(startingGroup3);
        saved.push(startingGroup4);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("nested_stack", saved);
        }

        WHEN("loading")
        {
            NestedIntegers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("nested_stack", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved stack with objects")
    {
        const auto objects = dataGeneration.RandomStackGroup<TestClass, int, std::string>(5);

        Classes saved;
        saved.push(objects[0]);
        saved.push(objects[1]);
        saved.push(objects[2]);
        saved.push(objects[3]);
        saved.push(objects[4]);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("stack", saved);
        }

        WHEN("loading")
        {
            Classes loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("stack", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }
}