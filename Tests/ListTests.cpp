#include <catch.hpp>

#include <Inscription/ListScribe.h>
#include <Inscription/NumericScribe.h>

#include "GeneralFixture.h"

#include "TestClass.h"

class ListTestsFixture : public GeneralFixture
{
public:
    using Integers = std::list<int>;
    using NestedIntegers = std::list<std::list<int>>;
    using Classes = std::list<TestClass>;
};

SCENARIO_METHOD(ListTestsFixture, "loading list after saving binary", "[binary][container][list]")
{
    GIVEN("saved empty list")
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

    GIVEN("saved single element list")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(1);

        for (auto& loop : startingGroup)
            saved.push_front(loop);

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

    GIVEN("saved list")
    {
        const auto startingGroup = dataGeneration.RandomGroup<int>(5);

        const Integers saved(startingGroup.begin(), startingGroup.end());

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
            const auto occupiedGroup = dataGeneration.RandomGroup<int>(5);

            Integers loaded(occupiedGroup.begin(), occupiedGroup.end());

            Inscription::Binary::FromFile(loaded, "Test.dat");

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved nested lists")
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

    GIVEN("saved list with objects")
    {
        const auto objects = dataGeneration.RandomStackGroup<TestClass, int, std::string>(5);

        const Classes saved{ objects.begin(), objects.end() };

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

SCENARIO_METHOD(ListTestsFixture, "loading list after saving json", "[json][container][list]")
{
    GIVEN("saved empty list")
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

    GIVEN("saved single element list")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(1);

        for (auto& loop : startingGroup)
            saved.push_front(loop);

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

    GIVEN("saved list")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);

        for (auto& loop : startingGroup)
            saved.push_front(loop);

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
            const auto occupiedGroup = dataGeneration.RandomGroup<int>(5);

            Integers loaded(occupiedGroup.begin(), occupiedGroup.end());

            Inscription::Json::FromFile(loaded, "Test.json");

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved nested lists")
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

    GIVEN("saved list with objects")
    {
        const auto objects = dataGeneration.RandomStackGroup<TestClass, int, std::string>(5);

        const Classes saved{ objects.begin(), objects.end() };

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