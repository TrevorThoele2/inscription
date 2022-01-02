#include <catch.hpp>

#include <Inscription/TupleScribe.h>
#include <Inscription/StringScribe.h>

#include "GeneralFixture.h"

class TupleTestsFixture : public GeneralFixture
{
public:
    using TestedObject = std::tuple<int, std::string, unsigned short>;
};

SCENARIO_METHOD(TupleTestsFixture, "loading tuple after saving binary", "[binary][std][tuple]")
{
    GIVEN("saved tuple")
    {
        auto saved = dataGeneration.RandomStack<TestedObject, int, std::string, unsigned short>();

        Inscription::Binary::ToFile(saved, "Test.dat");

        WHEN("loading tuple")
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

SCENARIO_METHOD(TupleTestsFixture, "loading tuple after saving json", "[json][std][tuple]")
{
    using TestedObject = std::tuple<int, std::string, unsigned short>;

    GIVEN("saved tuple")
    {
        auto saved = dataGeneration.RandomStack<TestedObject, int, std::string, unsigned short>();

        Inscription::Json::ToFile(saved, "Test.json");

        WHEN("loading tuple")
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