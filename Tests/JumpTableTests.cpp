#include <catch.hpp>

#include <Inscription/OutputJumpTable.h>
#include <Inscription/InputJumpTable.h>

#include <Inscription/OutputBinaryArchive.h>
#include <Inscription/InputBinaryArchive.h>

#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"

class JumpTableFixture : public BinaryFixture
{
public:
    struct Object
    {
        int i = 0;
        short s = 0;
        std::string string;
    };
};

SCENARIO_METHOD(JumpTableFixture, "default jump table", "[jumptable]")
{
    GIVEN("default save")
    {
        const ::Inscription::OutputJumpTable<int, int> jumpTable;

        WHEN("querying all ids")
        {
            auto allIDs = jumpTable.AllIDs();

            THEN("is empty")
            {
                REQUIRE(allIDs.empty());
            }
        }
    }

    GIVEN("default load")
    {
        const ::Inscription::InputJumpTable<int, int> jumpTable;

        WHEN("querying all ids")
        {
            auto allIDs = jumpTable.AllIDs();

            THEN("is empty")
            {
                REQUIRE(allIDs.empty());
            }
        }
    }
}

SCENARIO_METHOD(JumpTableFixture, "saving jump table", "[jumptable]")
{
    GIVEN("integer jump table")
    {
        auto savedIntegerIDs = dataGeneration.RandomGroup<int>(10, TestFramework::Range<int>(-100, 100));
        auto savedIntegerValues = dataGeneration.RandomGroup<int>(10, TestFramework::Range<int>(-10000, 10000));

        auto outputArchive = std::make_unique<::Inscription::OutputBinaryArchive>("Test.dat");

        ::Inscription::OutputJumpTable<int, int> savedJumpTable;

        WHEN("adding entry")
        {
            auto added1 = savedJumpTable.Add(savedIntegerIDs[0], savedIntegerValues[0]);

            THEN("returns true")
            {
                REQUIRE(added1 == true);
            }

            THEN("all ids contain id")
            {
                auto allIds = savedJumpTable.AllIDs();
                REQUIRE(allIds.size() == 1);
                REQUIRE(allIds[0] == savedIntegerIDs[0]);
            }

            WHEN("adding entry with duplicate id")
            {
                auto added2 = savedJumpTable.Add(savedIntegerIDs[0], savedIntegerValues[0]);

                THEN("returns false")
                {
                    REQUIRE(added2 == false);
                }

                THEN("all ids only contain one id")
                {
                    auto allIds = savedJumpTable.AllIDs();
                    REQUIRE(allIds.size() == 1);
                    REQUIRE(allIds[0] == savedIntegerIDs[0]);
                }

                THEN("saving and loading returns only one entry")
                {
                    (*outputArchive)(savedJumpTable);

                    outputArchive.reset();

                    ::Inscription::InputJumpTable<int, int> loadedJumpTable;

                    auto inputArchive = ::Inscription::InputBinaryArchive("Test.dat");
                    inputArchive(loadedJumpTable);

                    REQUIRE(loadedJumpTable.AllIDs().size() == 1);
                }
            }
        }
    }
}

SCENARIO_METHOD(JumpTableFixture, "loading jump table", "[jumptable]")
{
    GIVEN("saved integer jump table")
    {
        auto savedIntegerIDs = dataGeneration.RandomGroup<int>(10, TestFramework::Range<int>(-100, 100));
        auto savedIntegerValues = dataGeneration.RandomGroup<int>(10, TestFramework::Range<int>(-10000, 10000));

        auto outputArchive = std::make_unique<::Inscription::OutputBinaryArchive>("Test.dat");

        {
            ::Inscription::OutputJumpTable<int, int> savedJumpTable;
            for (size_t loop = 0; loop < savedIntegerIDs.size(); ++loop)
                savedJumpTable.Add(savedIntegerIDs[loop], savedIntegerValues[loop]);

            (*outputArchive)(savedJumpTable);
        }

        WHEN("loading integer jump table")
        {
            outputArchive.reset();

            ::Inscription::InputJumpTable<int, int> loadedJumpTable;

            auto inputArchive = ::Inscription::InputBinaryArchive("Test.dat");
            inputArchive(loadedJumpTable);

            THEN("all objects fillable")
            {
                for (size_t loop = 0; loop < savedIntegerIDs.size(); ++loop)
                {
                    int object = 0;
                    auto wasLoaded = loadedJumpTable.FillObject(savedIntegerIDs[loop], object, inputArchive);
                    REQUIRE(wasLoaded == true);
                    REQUIRE(object == savedIntegerValues[loop]);
                }
            }

            THEN("all ids are same as saved")
            {
                auto allIDs = loadedJumpTable.AllIDs();
                REQUIRE(allIDs.size() == savedIntegerIDs.size());
                for (auto savedIntegerID : savedIntegerIDs)
                {
                    auto found = std::find(allIDs.begin(), allIDs.end(), savedIntegerID);
                    REQUIRE(found != allIDs.end());
                }
            }

            THEN("filling object with invalid id")
            {
                int object = std::numeric_limits<int>::max();
                auto wasLoaded = loadedJumpTable.FillObject(std::numeric_limits<int>::min(), object, inputArchive);
                REQUIRE(wasLoaded == false);
                REQUIRE(object == std::numeric_limits<int>::max());
            }
        }

        WHEN("saving after table")
        {
            auto savedPostTable = dataGeneration.RandomGroup<std::string>(10);

            for(auto& loop : savedPostTable)
                (*outputArchive)(loop);

            WHEN("loading integer jump table")
            {
                outputArchive.reset();

                ::Inscription::InputJumpTable<int, int> loadedJumpTable;

                auto inputArchive = ::Inscription::InputBinaryArchive("Test.dat");
                inputArchive(loadedJumpTable);

                THEN("can load all post table data")
                {
                    for (const auto& loop : savedPostTable)
                    {
                        std::string postTable;
                        inputArchive(postTable);
                        REQUIRE(postTable == loop);
                    }
                }

                THEN("can interlace loading calls between table and post table")
                {
                    for(size_t loop = 0; loop < savedPostTable.size(); ++loop)
                    {
                        int object = 0;
                        loadedJumpTable.FillObject(savedIntegerIDs[loop], object, inputArchive);
                        REQUIRE(object == savedIntegerValues[loop]);

                        std::string postTable;
                        inputArchive(postTable);
                        REQUIRE(postTable == savedPostTable[loop]);
                    }
                }
            }
        }
    }
}