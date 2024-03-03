#include <catch.hpp>

#include <Inscription/EnumScribeCategory.h>

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
    template<class Archive>
    struct ScribeTraits<BinaryEnumFixture::Enum, Archive> final
    {
        using Category = EnumScribeCategory<BinaryEnumFixture::Enum>;
    };
}

SCENARIO_METHOD(BinaryEnumFixture, "loading enum after saving binary", "[binary][enum]")
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