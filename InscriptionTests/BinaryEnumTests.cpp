#include <boost/test/unit_test.hpp>

#include <Inscription/EnumScribe.h>

#include "BinaryFixture.h"

class BinaryEnumFixture : public BinaryFixture
{
public:
    enum Enum
    {
        ONE,
        TWO,
        THREE
    };
};

namespace Inscription
{
    template<>
    class Scribe<::BinaryEnumFixture::Enum, BinaryArchive> :
        public EnumScribe<::BinaryEnumFixture::Enum, BinaryArchive>
    {};
}

BOOST_FIXTURE_TEST_SUITE(BinaryEnumTests, BinaryEnumFixture)

BOOST_AUTO_TEST_CASE(SavesAndLoads)
{
    Enum saved = Enum::TWO;

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(saved);
    }

    Enum loaded;

    {
        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loaded);
    }

    BOOST_REQUIRE(loaded == saved);
}

BOOST_AUTO_TEST_SUITE_END()