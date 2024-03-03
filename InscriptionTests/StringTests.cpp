#include <boost/test/unit_test.hpp>

#include <Inscription/String.h>

#include "BinaryFixture.h"

class StringTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(StringTests, StringTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    auto saved = dataGeneration.Generator<std::string>().Random();

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(saved);
    }

    std::string loaded;

    {
        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loaded);
    }

    BOOST_REQUIRE(loaded == saved);
}

BOOST_AUTO_TEST_SUITE_END()