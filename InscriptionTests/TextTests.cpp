#include <boost/test/unit_test.hpp>

#include <Inscription/StringScribe.h>

#include "TextFixture.h"

class TextTestsFixture : public TextFixture
{};

BOOST_FIXTURE_TEST_SUITE(TextTests, TextTestsFixture)

BOOST_AUTO_TEST_CASE(String_SavesAndLoads)
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

    BOOST_REQUIRE(loaded == saved);
}

BOOST_AUTO_TEST_CASE(String_LoadsAsLine)
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

    BOOST_REQUIRE(loadedFirst == savedFirst);
    BOOST_REQUIRE(loadedSecond == savedSecond);
}

BOOST_AUTO_TEST_SUITE_END()