#include <catch.hpp>

#include <Inscription/FilePathScribe.h>

#include "GeneralFixture.h"

class FilePathTestsFixture : public GeneralFixture
{};

SCENARIO_METHOD(FilePathTestsFixture, "loading file path after save binary", "[binary][std][filepath]")
{
    GIVEN("saved file path")
    {
        std::filesystem::path saved = dataGeneration.Random<std::string>();

        Inscription::Binary::ToFile(saved, "Test.dat");

        WHEN("loading file path")
        {
            std::filesystem::path loaded;

            Inscription::Binary::FromFile(loaded, "Test.dat");

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}

SCENARIO_METHOD(FilePathTestsFixture, "loading file path after save json", "[json][std][filepath]")
{
    GIVEN("saved file path")
    {
        std::filesystem::path saved = dataGeneration.Random<std::string>();

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading file path")
        {
            std::filesystem::path loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}