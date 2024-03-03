#include <catch.hpp>

#include <Inscription/EnumScribe.h>

#include "BinaryFixture.h"

class BinaryEnumFixture : public BinaryFixture
{
public:
    enum Enum
    {
        One,
        Two,
        Three
    };
};

namespace Inscription
{
    template<>
    class Scribe<::BinaryEnumFixture::Enum, BinaryArchive> :
        public EnumScribe<::BinaryEnumFixture::Enum, BinaryArchive>
    {};
}

SCENARIO_METHOD(BinaryEnumFixture, "loading binary enum", "[binary][enum]")
{
    GIVEN("saved enum")
    {
        Enum saved = Two;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading enum")
        {
            Enum loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("enum is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}