#include <catch.hpp>

#include <Inscription/OutputJumpTable.h>
#include <Inscription/InputJumpTable.h>

#include <Inscription/OutputBinaryArchive.h>
#include <Inscription/InputBinaryArchive.h>

#include <Inscription/CompositeScribe.h>
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

SCENARIO_METHOD(JumpTableFixture, "loading jump table", "[jumptable]")
{
    GIVEN("saved integer jump table")
    {
        auto savedIntegerIDs = dataGeneration.RandomGroup<int>(10);
        auto savedIntegerValues = dataGeneration.RandomGroup<int>(10);

        auto outputArchive = std::make_unique<::Inscription::OutputBinaryArchive>("Test.dat", "testing", 1);

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

            auto inputArchive = ::Inscription::InputBinaryArchive("Test.dat", "testing");
            inputArchive(loadedJumpTable);

            THEN("all objects are constructible")
            {
                for (size_t loop = 0; loop < savedIntegerIDs.size(); ++loop)
                {
                    auto object = loadedJumpTable.ConstructObject(savedIntegerIDs[loop], inputArchive);
                    REQUIRE(object == savedIntegerValues[loop]);
                }
            }

            THEN("all objects fillable")
            {
                for (size_t loop = 0; loop < savedIntegerIDs.size(); ++loop)
                {
                    int object;
                    loadedJumpTable.FillObject(object, savedIntegerIDs[loop], inputArchive);
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

                auto inputArchive = ::Inscription::InputBinaryArchive("Test.dat", "testing");
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
                        auto object = loadedJumpTable.ConstructObject(savedIntegerIDs[loop], inputArchive);
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