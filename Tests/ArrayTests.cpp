#include <catch.hpp>

#include <Inscription/ArrayScribe.h>
#include <Inscription/NumericScribe.h>

#include "BinaryFixture.h"
#include "JsonFixture.h"

#include "TestClass.h"

class ArrayBinaryTestsFixture : public BinaryFixture
{
public:
    using Integers = std::array<int, 5>;
    using NestedIntegers = std::array<std::array<int, 5>, 5>;
    using Classes = std::array<TestClass, 5>;
};

class ArrayJsonTestsFixture : public JsonFixture
{
public:
    using Integers = std::array<int, 5>;
    using NestedIntegers = std::array<std::array<int, 5>, 5>;
    using Classes = std::array<TestClass, 5>;
};

SCENARIO_METHOD(ArrayBinaryTestsFixture, "loading array after saving binary", "[binary][container][array]")
{
    GIVEN("saved array")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(saved.size());

        for (size_t i = 0; i < startingGroup.size(); ++i)
            saved[i] = startingGroup[i];

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading array")
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

            const auto occupiedGroup = dataGeneration.RandomGroup<int>(saved.size());

            for (size_t i = 0; i < occupiedGroup.size(); ++i)
                loaded[i] = occupiedGroup[i];

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved nested arrays")
    {
        const auto startingGroup0 = dataGeneration.RandomGroup<int>(5);
        const auto startingGroup1 = dataGeneration.RandomGroup<int>(5);
        const auto startingGroup2 = dataGeneration.RandomGroup<int>(5);
        const auto startingGroup3 = dataGeneration.RandomGroup<int>(5);
        const auto startingGroup4 = dataGeneration.RandomGroup<int>(5);

        const NestedIntegers saved
        {
            Integers {
                startingGroup4[0],
                startingGroup4[1],
                startingGroup4[2],
                startingGroup4[3],
                startingGroup4[4]
            },
            {
                startingGroup1[0],
                startingGroup1[1],
                startingGroup1[2],
                startingGroup1[3],
                startingGroup1[4]
            },
            {
                startingGroup2[0],
                startingGroup2[1],
                startingGroup2[2],
                startingGroup2[3],
                startingGroup2[4]
            },
            {
                startingGroup3[0],
                startingGroup3[1],
                startingGroup3[2],
                startingGroup3[3],
                startingGroup3[4]
            },
            {
                startingGroup4[0],
                startingGroup4[1],
                startingGroup4[2],
                startingGroup4[3],
                startingGroup4[4]
            }
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

    GIVEN("saved array with objects")
    {
        const auto objects = dataGeneration.RandomStackGroup<TestClass, int, std::string>(5);

        const Classes saved
        {
            objects[0],
            objects[1],
            objects[2],
            objects[3],
            objects[4]
        };

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

SCENARIO_METHOD(ArrayJsonTestsFixture, "loading array after saving json", "[json][container][array]")
{
    GIVEN("saved array")
    {
        Integers saved;

        const auto startingGroup = dataGeneration.RandomGroup<int>(saved.size());

        for (size_t i = 0; i < startingGroup.size(); ++i)
            saved[i] = startingGroup[i];

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("array", saved);
        }

        WHEN("loading array")
        {
            Integers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("array", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }

        WHEN("loading over occupied container")
        {
            Integers loaded;

            const auto occupiedGroup = dataGeneration.RandomGroup<int>(saved.size());

            for (size_t i = 0; i < occupiedGroup.size(); ++i)
                loaded[i] = occupiedGroup[i];

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("array", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved nested arrays")
    {
        const auto startingGroup0 = dataGeneration.RandomGroup<int>(5);
        const auto startingGroup1 = dataGeneration.RandomGroup<int>(5);
        const auto startingGroup2 = dataGeneration.RandomGroup<int>(5);
        const auto startingGroup3 = dataGeneration.RandomGroup<int>(5);
        const auto startingGroup4 = dataGeneration.RandomGroup<int>(5);

        const NestedIntegers saved
        {
            Integers {
                startingGroup4[0],
                startingGroup4[1],
                startingGroup4[2],
                startingGroup4[3],
                startingGroup4[4]
            },
            {
                startingGroup1[0],
                startingGroup1[1],
                startingGroup1[2],
                startingGroup1[3],
                startingGroup1[4]
            },
            {
                startingGroup2[0],
                startingGroup2[1],
                startingGroup2[2],
                startingGroup2[3],
                startingGroup2[4]
            },
            {
                startingGroup3[0],
                startingGroup3[1],
                startingGroup3[2],
                startingGroup3[3],
                startingGroup3[4]
            },
            {
                startingGroup4[0],
                startingGroup4[1],
                startingGroup4[2],
                startingGroup4[3],
                startingGroup4[4]
            }
        };

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("nested_array", saved);
        }

        WHEN("loading")
        {
            NestedIntegers loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("nested_array", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }

    GIVEN("saved array with objects")
    {
        const auto objects = dataGeneration.RandomStackGroup<TestClass, int, std::string>(5);

        const Classes saved
        {
            objects[0],
            objects[1],
            objects[2],
            objects[3],
            objects[4]
        };

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("array", saved);
        }

        WHEN("loading")
        {
            Classes loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("array", loaded);

            THEN("is same as saved")
            {
                REQUIRE(!loaded.empty());
                REQUIRE(loaded == saved);
            }
        }
    }
}