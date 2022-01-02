#include <catch.hpp>

#include <Inscription/OutputJumpTable.h>
#include <Inscription/InputJumpTable.h>

#include <Inscription/OutputBinaryArchive.h>
#include <Inscription/InputBinaryArchive.h>

#include <Inscription/StringScribe.h>

#include "GeneralFixture.h"

class JumpTableFixture : public GeneralFixture
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
        const Inscription::OutputJumpTable<int, int> jumpTable;

        WHEN("querying all ids")
        {
            const auto allIDs = jumpTable.AllIDs();

            THEN("is empty")
            {
                REQUIRE(allIDs.empty());
            }
        }
    }

    GIVEN("default load")
    {
        const Inscription::InputJumpTable<int, int> jumpTable;

        WHEN("querying all ids")
        {
            const auto allIDs = jumpTable.AllIDs();

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
        auto savedIntegerIDs = dataGeneration.RandomGroup<int>(10, TestFramework::Range(-100, 100));
        auto savedIntegerValues = dataGeneration.RandomGroup<int>(10, TestFramework::Range(-10000, 10000));

        auto outputFile = std::make_unique<Inscription::File::OutputBinary>("Test.dat");
        auto outputArchive = std::make_unique<Inscription::Archive::OutputBinary>(*outputFile);
        auto outputFormat = std::make_unique<Inscription::Format::OutputBinary>(*outputArchive);

        Inscription::OutputJumpTable<int, int> savedJumpTable;

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
                    savedJumpTable.Save(*outputFile, *outputFormat);

                    outputFormat.reset();
                    outputArchive.reset();
                    outputFile.reset();

                    auto inputFile = std::make_unique<Inscription::File::InputBinary>("Test.dat");
                    auto inputArchive = std::make_unique<Inscription::Archive::InputBinary>(*inputFile);
                    auto inputFormat = std::make_unique<Inscription::Format::InputBinary>(*inputArchive);

                    Inscription::InputJumpTable<int, int> loadedJumpTable;
                    loadedJumpTable.Load(*inputFile, *inputFormat);

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

        auto outputFile = std::make_unique<Inscription::File::OutputBinary>("Test.dat");
        auto outputArchive = std::make_unique<Inscription::Archive::OutputBinary>(*outputFile);
        auto outputFormat = std::make_unique<Inscription::Format::OutputBinary>(*outputArchive);

        {
            Inscription::OutputJumpTable<int, int> savedJumpTable;
            for (size_t loop = 0; loop < savedIntegerIDs.size(); ++loop)
                savedJumpTable.Add(savedIntegerIDs[loop], savedIntegerValues[loop]);

            savedJumpTable.Save(*outputFile, *outputFormat);
        }

        WHEN("loading integer jump table")
        {
            outputArchive.reset();

            Inscription::InputJumpTable<int, int> loadedJumpTable;

            auto inputFile = std::make_unique<Inscription::File::InputBinary>("Test.dat");
            auto inputArchive = std::make_unique<Inscription::Archive::InputBinary>(*inputFile);
            auto inputFormat = std::make_unique<Inscription::Format::InputBinary>(*inputArchive);

            loadedJumpTable.Load(*inputFile, *inputFormat);

            THEN("all objects fillable")
            {
                for (size_t loop = 0; loop < savedIntegerIDs.size(); ++loop)
                {
                    int object = 0;
                    auto wasLoaded = loadedJumpTable.FillObject(savedIntegerIDs[loop], object, *inputFormat);
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
                auto wasLoaded = loadedJumpTable.FillObject(std::numeric_limits<int>::min(), object, *inputFormat);
                REQUIRE(wasLoaded == false);
                REQUIRE(object == std::numeric_limits<int>::max());
            }
        }

        WHEN("saving after table")
        {
            auto savedPostTable = dataGeneration.RandomGroup<std::string>(10);

            for(auto& loop : savedPostTable)
                (*outputFormat)(loop);

            WHEN("loading integer jump table")
            {
                outputFormat.reset();
                outputArchive.reset();
                outputFile.reset();

                Inscription::InputJumpTable<int, int> loadedJumpTable;

                auto inputFile = std::make_unique<Inscription::File::InputBinary>("Test.dat");
                auto inputArchive = std::make_unique<Inscription::Archive::InputBinary>(*inputFile);
                auto inputFormat = std::make_unique<Inscription::Format::InputBinary>(*inputArchive);

                loadedJumpTable.Load(*inputFile, *inputFormat);

                THEN("can load all post table data")
                {
                    for (const auto& loop : savedPostTable)
                    {
                        std::string postTable;
                        (*inputFormat)(postTable);
                        REQUIRE(postTable == loop);
                    }
                }

                THEN("can interlace loading calls between table and post table")
                {
                    for(size_t loop = 0; loop < savedPostTable.size(); ++loop)
                    {
                        int object = 0;
                        loadedJumpTable.FillObject(savedIntegerIDs[loop], object, *inputFormat);
                        REQUIRE(object == savedIntegerValues[loop]);

                        std::string postTable;
                        (*inputFormat)(postTable);
                        REQUIRE(postTable == savedPostTable[loop]);
                    }
                }
            }
        }
    }
}