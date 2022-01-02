#include <catch.hpp>

#include <Inscription/StackScribe.h>
#include <Inscription/NumericScribe.h>

#include "GeneralFixture.h"

#include "TestClass.h"

class StackTestsFixture : public GeneralFixture
{
public:
    using Integers = std::stack<int>;
    using NestedIntegers = std::stack<std::stack<int>>;
    using Classes = std::stack<TestClass>;
};

SCENARIO_METHOD(StackTestsFixture, "loading stack after saving binary", "[binary][container][stack]")
{
    GIVEN("saved empty stack")
    {
        Integers saved;

        Inscription::Binary::ToFile(saved, "Test.dat");

        WHEN("loading")
        {
            Integers loaded;

            Inscription::Binary::FromFile(loaded, "Test.dat");

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

        Inscription::Binary::ToFile(saved, "Test.dat");

        WHEN("loading")
        {
            Integers loaded;

            Inscription::Binary::FromFile(loaded, "Test.dat");

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

        Inscription::Binary::ToFile(saved, "Test.dat");

        WHEN("loading")
        {
            Integers loaded;

            Inscription::Binary::FromFile(loaded, "Test.dat");

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

            Inscription::Binary::FromFile(loaded, "Test.dat");

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

        Inscription::Binary::ToFile(saved, "Test.dat");

        WHEN("loading")
        {
            NestedIntegers loaded;

            Inscription::Binary::FromFile(loaded, "Test.dat");

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

        Inscription::Binary::ToFile(saved, "Test.dat");

        WHEN("loading")
        {
            Classes loaded;

            Inscription::Binary::FromFile(loaded, "Test.dat");

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }
}

SCENARIO_METHOD(StackTestsFixture, "loading stack after saving json", "[json][container][stack]")
{
    GIVEN("saved empty stack")
    {
        Integers saved;

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading")
        {
            Integers loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

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

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading")
        {
            Integers loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

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

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading")
        {
            Integers loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

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

            Inscription::Json::FromFile(loaded, "Test.json");

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

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading")
        {
            NestedIntegers loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

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

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading")
        {
            Classes loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }
}