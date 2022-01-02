#include <catch.hpp>

#include <Inscription/EnumScribeCategory.h>

#include "GeneralFixture.h"

class JsonEnumFixture : public GeneralFixture
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
    struct ScribeTraits<JsonEnumFixture::Enum, Format> final
    {
        using Category = EnumScribeCategory<JsonEnumFixture::Enum>;
    };
}

SCENARIO_METHOD(JsonEnumFixture, "loading enum after saving json", "[json][enum]")
{
    GIVEN("saved enum")
    {
        auto saved = Two;

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading enum")
        {
            Enum loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

            THEN("enum is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}