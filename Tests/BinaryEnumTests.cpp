#include <catch.hpp>

#include <Inscription/EnumScribeCategory.h>

#include "GeneralFixture.h"

class BinaryEnumFixture : public GeneralFixture
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
    template<class Format>
    struct ScribeTraits<BinaryEnumFixture::Enum, Format> final
    {
        using Category = EnumScribeCategory<BinaryEnumFixture::Enum>;
    };
}

SCENARIO_METHOD(BinaryEnumFixture, "loading enum after saving binary", "[binary][enum]")
{
    GIVEN("saved enum")
    {
        Enum saved = Two;

        Inscription::Binary::ToFile(saved, "Test.dat");

        WHEN("loading enum")
        {
            Enum loaded;

            Inscription::Binary::FromFile(loaded, "Test.dat");

            THEN("enum is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}