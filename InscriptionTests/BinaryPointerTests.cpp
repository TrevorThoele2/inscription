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
    BinaryPointerFixture()
    {
        typeRegistrationContext.RegisterType<Object>();
    }

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
};

namespace Inscription
{
    template<>
    class Scribe<::BinaryPointerFixture::Object, BinaryArchive> : public
        CompositeScribe<::BinaryPointerFixture::Object, BinaryArchive>
    {
    public:
        static void ScrivenImplementation(ObjectT& object, ArchiveT& archive)
        {
            archive(object.value);
        }
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

BOOST_AUTO_TEST_CASE(Loads_ObjectSavedBeforehand)
{
    Object savedObject = dataGeneration.Generator<Object>().RandomStack<int>();
    Object* savedPointer = &savedObject;

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(savedObject);
        outputArchive(savedPointer);
    }

    Object loadedObject;
    Object* loadedPointer;

    {
        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loadedObject);
        inputArchive(loadedPointer);
    }

    BOOST_REQUIRE(loadedObject.value == savedObject.value);
    BOOST_REQUIRE(loadedPointer == &loadedObject);
}

BOOST_AUTO_TEST_CASE(OwnedLoad_CausesUnownedToBePopulated)
{
    Object* savedOwned = dataGeneration.Generator<Object>().RandomHeap<int>();
    Object* savedUnowned = savedOwned;

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(savedUnowned, ::Inscription::Pointer::Unowning);
        outputArchive(savedOwned, ::Inscription::Pointer::Owning);
    }

    Object* loadedOwned = nullptr;
    Object* loadedUnowned = nullptr;

    {
        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loadedUnowned, ::Inscription::Pointer::Unowning);
        inputArchive(loadedOwned, ::Inscription::Pointer::Owning);
    }

    BOOST_REQUIRE(loadedUnowned != nullptr);
    BOOST_REQUIRE(loadedOwned != nullptr);
    BOOST_REQUIRE(loadedOwned == loadedUnowned);
    BOOST_REQUIRE(loadedOwned->value == savedOwned->value);

    delete savedOwned;
    delete loadedOwned;
}

BOOST_AUTO_TEST_SUITE_END()