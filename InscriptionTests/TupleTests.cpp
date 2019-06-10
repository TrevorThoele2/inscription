#include <boost/test/unit_test.hpp>

#include <Inscription/Tuple.h>
#include <Inscription/Numeric.h>
#include <Inscription/String.h>

#include "BinaryFixture.h"

class TupleTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(TupleTests, TupleTestsFixture)

BOOST_AUTO_TEST_CASE(BasicTuple_SavesAndLoads)
{
    typedef std::tuple<int, std::string, unsigned short> TestedObject;
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