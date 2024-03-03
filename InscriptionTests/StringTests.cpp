#include <boost/test/unit_test.hpp>

#include <Inscription/String.h>

#include "BinaryFixture.h"

class StringTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(StringTests, StringTestsFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    auto string = dataGeneration.Generator<std::string>().Random();

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(string);
    }

    std::string n_string;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_string);
    }

    BOOST_REQUIRE(n_string == string);
}

BOOST_AUTO_TEST_SUITE_END()