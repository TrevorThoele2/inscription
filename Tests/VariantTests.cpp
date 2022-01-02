#include <catch.hpp>

#include <Inscription/VariantScribe.h>
#include <Inscription/StringScribe.h>

#include "GeneralFixture.h"

class VariantTestsFixture : public GeneralFixture
{
public:
    using TestedObject = std::variant<int, std::string, unsigned short>;
};

SCENARIO_METHOD(VariantTestsFixture, "loading variant after saving binary", "[binary][std][variant]")
{
    GIVEN("saved variant")
    {
        auto saved = dataGeneration.RandomStack<TestedObject, std::string>();

        Inscription::Binary::ToFile(saved, "Test.dat");

        WHEN("loading variant")
        {
            TestedObject loaded;

            Inscription::Binary::FromFile(loaded, "Test.dat");

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}

SCENARIO_METHOD(VariantTestsFixture, "loading variant after saving json", "[json][std][variant]")
{
    GIVEN("saved variant")
    {
        auto saved = dataGeneration.RandomStack<TestedObject, std::string>();

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading variant")
        {
            TestedObject loaded;

            Inscription::Json::FromFile(loaded, "Test.json");

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}