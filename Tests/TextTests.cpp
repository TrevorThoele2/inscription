#include <catch.hpp>

#include <Inscription/StringScribe.h>

#include "TextFixture.h"

class TextTestsFixture : public TextFixture
{};

TEST_CASE_METHOD(TextTestsFixture, "text")
{
    SECTION("string saves and loads")
    {
        std::string saved = dataGeneration.Generator<std::string>().Random();

        {
            auto outputArchive = CreateOutputArchive();
            outputArchive(saved);
        }

        std::string loaded;

        {
            auto inputArchive = CreateInputArchive();
            inputArchive(loaded);
        }

        REQUIRE(loaded == saved);
    }

    SECTION("string loads as line")
    {
        std::string savedFirst = dataGeneration.Generator<std::string>().Random();
        std::string savedSecond = dataGeneration.Generator<std::string>().Random();

        {
            std::string totalSaved = savedFirst + "\n" + savedSecond;

            auto outputArchive = CreateOutputArchive();
            outputArchive(totalSaved);
        }

        std::string loadedFirst;
        std::string loadedSecond;

        {
            auto inputArchive = CreateInputArchive();
            inputArchive(loadedFirst);
            inputArchive(loadedSecond);
        }

        REQUIRE(loadedFirst == savedFirst);
        REQUIRE(loadedSecond == savedSecond);
    }
}