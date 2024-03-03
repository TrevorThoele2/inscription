#include <catch.hpp>

#include <Inscription/QueueScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"
#include "JsonFixture.h"

#include "TestClass.h"

class QueueBinaryTestsFixture : public BinaryFixture
{
public:
    using Integers = std::queue<int>;
    using NestedIntegers = std::queue<std::queue<int>>;
    using Classes = std::queue<TestClass>;
};

class QueueJsonTestsFixture : public JsonFixture
{
public:
    using Integers = std::queue<int>;
    using NestedIntegers = std::queue<std::queue<int>>;
    using Classes = std::queue<TestClass>;
};

SCENARIO_METHOD(QueueBinaryTestsFixture, "loading queue after saving binary", "[binary][container][queue]")
{
    GIVEN("saved empty queue")
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

    GIVEN("saved single element queue")
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

    GIVEN("saved queue")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);

        for (auto& loop : startingGroup)
            saved.push(loop);

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
                loaded.push(loop);

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved nested queues")
    {
        const auto constructGroup = [this]()
        {
            const auto group = dataGeneration.RandomGroup<int>(5);
            Integers queue;
            for (auto& i : group)
                queue.push(i);
            return queue;
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

    GIVEN("saved queue with objects")
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

SCENARIO_METHOD(QueueJsonTestsFixture, "loading queue after saving json", "[json][container][queue]")
{
    GIVEN("saved empty queue")
    {
        Integers saved;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("empty_queue", saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("empty_queue", loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded.empty());
            }
        }
    }

    GIVEN("saved single element queue")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(1);

        for (auto& loop : startingGroup)
            saved.emplace(loop);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("queue", saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("queue", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved queue")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(5);

        for (auto& loop : startingGroup)
            saved.push(loop);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("queue", saved);
        }

        WHEN("loading")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("queue", loaded);

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
                loaded.push(loop);

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("queue", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved nested queues")
    {
        const auto constructGroup = [this]()
        {
            const auto group = dataGeneration.RandomGroup<int>(5);
            Integers queue;
            for (auto& i : group)
                queue.push(i);
            return queue;
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
            outputArchive("nested_queue", saved);
        }

        WHEN("loading")
        {
            NestedIntegers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("nested_queue", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved queue with objects")
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
            outputArchive("queue", saved);
        }

        WHEN("loading")
        {
            Classes loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("queue", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }
}