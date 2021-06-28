#include <catch.hpp>

#include <Inscription/StringScribe.h>

#include "TextFixture.h"

class TextTestsFixture : public TextFixture
{};

SCENARIO_METHOD(TextTestsFixture, "opening input text file", "[text]")
{
    GIVEN("nonexistent file")
    {
        WHEN("loading directly")
        {
            auto inputFile = Inscription::File::InputText("Nonexistent.txt");

            THEN("position is 0")
            {
                REQUIRE(inputFile.Position() == 0);
            }

            THEN("is at end of file")
            {
                REQUIRE(inputFile.IsAtEnd());
            }
        }
    }
}

SCENARIO_METHOD(TextTestsFixture, "loading strings from text archive", "[text]")
{
    GIVEN("three saved strings")
    {
        std::array<std::string, 3> saved =
        {
            dataGeneration.Random<std::string>(),
            dataGeneration.Random<std::string>(),
            dataGeneration.Random<std::string>()
        };

        {
            auto outputArchive = CreateOutputArchive();
            outputArchive(saved[0]);
            outputArchive(saved[1]);
            outputArchive(saved[2]);
        }

        WHEN("loading directly")
        {
            std::array<std::string, 3> loaded;

            {
                auto inputArchive = CreateInputArchive();
                inputArchive(loaded[0]);
                inputArchive(loaded[1]);
                inputArchive(loaded[2]);
            }

            THEN("all turns into a single string")
            {
                REQUIRE(loaded[0] == saved[0] + saved[1] + saved[2]);
                REQUIRE(loaded[1].empty());
                REQUIRE(loaded[2].empty());
            }
        }
    }

    GIVEN("three saved strings with newlines")
    {
        std::array<std::string, 3> saved =
        {
            dataGeneration.Random<std::string>() + "\n",
            dataGeneration.Random<std::string>() + "\n",
            dataGeneration.Random<std::string>() + "\n"
        };

        {
            auto outputArchive = CreateOutputArchive();
            outputArchive(saved[0]);
            outputArchive(saved[1]);
            outputArchive(saved[2]);
        }

        WHEN("loading as lines")
        {
            std::array<std::string, 3> loaded;

            {
                auto inputArchive = CreateInputArchive();
                inputArchive.ReadLine(loaded[0]);
                inputArchive.ReadLine(loaded[1]);
                inputArchive.ReadLine(loaded[2]);
            }

            THEN("loaded is same as saved")
            {
                auto expected = saved;
                expected[0].pop_back();
                expected[1].pop_back();
                expected[2].pop_back();
                REQUIRE(loaded == expected);
            }
        }
    }
}