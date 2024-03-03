#include <catch.hpp>

#include <Inscription/FilePathScribe.h>

#include "BinaryFixture.h"
#include "JsonFixture.h"

class FilePathBinaryTestsFixture : public BinaryFixture
{};

class FilePathJsonTestsFixture : public JsonFixture
{};

SCENARIO_METHOD(FilePathBinaryTestsFixture, "loading file path after save binary", "[binary][std][filepath]")
{
    GIVEN("saved occupied optional")
    {
        std::filesystem::path saved = dataGeneration.Random<std::string>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading optional")
        {
            std::filesystem::path loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}

SCENARIO_METHOD(FilePathJsonTestsFixture, "loading file path after save json", "[json][std][filepath]")
{
    GIVEN("saved occupied optional")
    {
        std::filesystem::path saved = dataGeneration.Random<std::string>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("filepath", saved);
        }

        WHEN("loading optional")
        {
            std::filesystem::path loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("filepath", loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}