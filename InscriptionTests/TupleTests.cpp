#include <boost/test/unit_test.hpp>

#include <Inscription/TupleScribe.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"

class TupleTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(TupleTests, TupleTestsFixture)

BOOST_AUTO_TEST_CASE(BasicTuple_SavesAndLoads)
{
    using TestedObject = std::tuple<int, std::string, unsigned short>;
    auto saved = dataGeneration.Generator<TestedObject>().RandomStack<int, std::string, unsigned short>();

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(saved);
    }

    TestedObject loaded;

    {
        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loaded);
    }

    BOOST_REQUIRE(loaded == saved);
}

BOOST_AUTO_TEST_SUITE_END()