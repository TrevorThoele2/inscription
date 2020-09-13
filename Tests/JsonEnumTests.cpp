#include <catch.hpp>

#include <Inscription/EnumScribe.h>

#include "JsonFixture.h"

class JsonEnumFixture : public JsonFixture
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
    class Scribe<::JsonEnumFixture::Enum, JsonArchive> final :
        public EnumScribe<::JsonEnumFixture::Enum, JsonArchive>
    {};
}

SCENARIO_METHOD(JsonEnumFixture, "loading enum after saving json", "[json][enum]")
{
    GIVEN("saved enum")
    {
        auto saved = Two;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("enum", saved);
        }

        WHEN("loading enum")
        {
            Enum loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("enum", loaded);

            THEN("enum is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}