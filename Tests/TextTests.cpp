#include <catch.hpp>

#include <Inscription/StringScribe.h>

#include "GeneralFixture.h"

class TextTestsFixture : public GeneralFixture
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

SCENARIO_METHOD(TextTestsFixture, "loading strings from text file", "[text]")
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
            auto file = Inscription::File::OutputText("Test.txt");
            auto archive = Inscription::Archive::OutputText(file);
            auto format = Inscription::Format::OutputPlaintext(archive);

            format(saved[0]);
            format(saved[1]);
            format(saved[2]);
        }

        WHEN("loading directly")
        {
            std::array<std::string, 3> loaded;

            {
                auto file = Inscription::File::InputText("Test.txt");
                auto archive = Inscription::Archive::InputText(file);
                auto format = Inscription::Format::InputPlaintext(archive);

                format(loaded[0]);
                format(loaded[1]);
                format(loaded[2]);
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
            auto file = Inscription::File::OutputText("Test.txt");
            auto archive = Inscription::Archive::OutputText(file);
            auto format = Inscription::Format::OutputPlaintext(archive);

            format(saved[0]);
            format(saved[1]);
            format(saved[2]);
        }

        WHEN("loading as lines")
        {
            std::array<std::string, 3> loaded;

            {
                auto file = Inscription::File::InputText("Test.txt");
                auto archive = Inscription::Archive::InputText(file);
                auto format = Inscription::Format::InputPlaintext(archive);

                format.ReadUntil(loaded[0], '\n');
                format.ReadUntil(loaded[1], '\n');
                format.ReadUntil(loaded[2], '\n');
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