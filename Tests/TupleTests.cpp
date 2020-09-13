#include <catch.hpp>

#include <Inscription/TupleScribe.h>
#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"
#include "JsonFixture.h"

class TupleTestsBinaryFixture : public BinaryFixture
{
public:
    using TestedObject = std::tuple<int, std::string, unsigned short>;
};

class TupleTestsJsonFixture : public JsonFixture
{
public:
    using TestedObject = std::tuple<int, std::string, unsigned short>;
};

SCENARIO_METHOD(TupleTestsBinaryFixture, "loading tuple after saving binary", "[binary][std][tuple]")
{
    GIVEN("saved tuple")
    {
        auto saved = dataGeneration.RandomStack<TestedObject, int, std::string, unsigned short>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading tuple")
        {
            TestedObject loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}

SCENARIO_METHOD(TupleTestsJsonFixture, "loading tuple after saving json", "[json][std][tuple]")
{
    using TestedObject = std::tuple<int, std::string, unsigned short>;

    GIVEN("saved tuple")
    {
        auto saved = dataGeneration.RandomStack<TestedObject, int, std::string, unsigned short>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("tuple", saved);
        }

        WHEN("loading tuple")
        {
            TestedObject loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("tuple", loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}