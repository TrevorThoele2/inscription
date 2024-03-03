#include <catch.hpp>

#include <Inscription/VariantScribe.h>
#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"
#include "JsonFixture.h"

class VariantBinaryTestsFixture : public BinaryFixture
{
public:
    using TestedObject = std::variant<int, std::string, unsigned short>;
};

class VariantJsonTestsFixture : public JsonFixture
{
public:
    using TestedObject = std::variant<int, std::string, unsigned short>;
};

SCENARIO_METHOD(VariantBinaryTestsFixture, "loading variant after saving binary", "[binary][std][variant]")
{
    GIVEN("saved variant")
    {
        auto saved = dataGeneration.RandomStack<TestedObject, std::string>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading variant")
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

SCENARIO_METHOD(VariantJsonTestsFixture, "loading variant after saving json", "[json][std][variant]")
{
    GIVEN("saved variant")
    {
        auto saved = dataGeneration.RandomStack<TestedObject, std::string>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("variant", saved);
        }

        WHEN("loading variant")
        {
            TestedObject loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive("variant", loaded);

            THEN("is same as saved")
            {
                REQUIRE(loaded == saved);
            }
        }
    }
}