#include <boost/test/unit_test.hpp>

#include <Inscription/TableScribe.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/PointerScribe.h>

#include <Inscription/MemoryScribe.h>
#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"

class BinaryPointerFixture : public BinaryFixture
{
public:
    class Object
    {
    public:
        int value = 0;
    public:
        Object()
        {}

        Object(int baseValue) : value(value)
        {}
    };

    class Unregistered
    {};
};

namespace Inscription
{
    template<>
    class Scribe<::BinaryPointerFixture::Object, BinaryArchive> : public
        CompositeScribe<::BinaryPointerFixture::Object, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive)
        {
            archive(object.value);
        }
    };

    template<>
    class Scribe<::BinaryPointerFixture::Unregistered, BinaryArchive> : public
        CompositeScribe<::BinaryPointerFixture::Unregistered, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive)
        {}
    };
}

BOOST_FIXTURE_TEST_SUITE(BinaryPointerTests, BinaryPointerFixture)

BOOST_AUTO_TEST_CASE(NullPointer_SavesAndLoads)
{
    Object* saved = nullptr;

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(saved);
    }

    Object* loaded = nullptr;

    {
        auto inputArchive = CreateRegistered<OutputArchive>();
        inputArchive(loaded);
    }

    BOOST_REQUIRE(loaded == nullptr);
}

BOOST_AUTO_TEST_CASE(UnregisteredType_ThrowsExceptionOnSave)
{
    Unregistered* saved = dataGeneration.Generator<Unregistered>().RandomHeap();

    auto outputArchive = CreateRegistered<OutputArchive>();

    BOOST_REQUIRE_THROW(outputArchive(saved), ::Inscription::RegisteredTypeNotFound);

    delete saved;
}

BOOST_AUTO_TEST_SUITE_END()