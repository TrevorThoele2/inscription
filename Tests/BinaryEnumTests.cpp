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

TEST_CASE_METHOD(BinaryEnumFixture, "binary enum")
{
    SECTION("save")
    {
        Enum saved = Two;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        SECTION("load")
        {
            Enum loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            REQUIRE(loaded == saved);
        }
    }
}