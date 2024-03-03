#include <boost/test/unit_test.hpp>

#include <Inscription/Tuple.h>

#include "BinaryFixture.h"

class TupleTestsFixture : public BinaryFixture
{};

BOOST_FIXTURE_TEST_SUITE(TupleTests, TupleTestsFixture)

BOOST_AUTO_TEST_CASE(BasicTuple_SavesAndLoads)
{
    typedef std::tuple<int, std::string, unsigned short> TestedObject;
    auto tuple = dataGeneration.Generator<TestedObject>().RandomStack<int, std::string, unsigned short>();

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(tuple);
    }

    TestedObject n_tuple;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_tuple);
    }

    BOOST_REQUIRE(n_tuple == tuple);
}

BOOST_AUTO_TEST_SUITE_END()