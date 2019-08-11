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

    class CyclicObject
    {
    public:
        int value = 0;
        CyclicObject* next = nullptr;
    public:
        CyclicObject()
        {}

        CyclicObject(int value) : value(value)
        {}
    };
};

namespace Inscription
{
    template<>
    class Scribe<::BinaryPointerFixture::Object, BinaryArchive> : public
        CompositeScribe<::BinaryPointerFixture::Object, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive(object.value);
        }

        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
        {
            DoBasicConstruction(storage, archive);
        }
    };

    template<>
    class Scribe<::BinaryPointerFixture::CyclicObject, BinaryArchive> : public
        CompositeScribe<::BinaryPointerFixture::CyclicObject, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive(object.value);
            archive(object.next);
        }

        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
        {
            DoBasicConstruction(storage, archive);
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

BOOST_AUTO_TEST_CASE(Loads_CyclicObject)
{
    CyclicObject* savedObject1 = dataGeneration.Generator<CyclicObject>().RandomHeap<int>();
    CyclicObject* savedObject2 = dataGeneration.Generator<CyclicObject>().RandomHeap<int>();
    savedObject1->next = savedObject2;
    savedObject2->next = savedObject1;

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(savedObject1);
    }

    CyclicObject* loadedObject1 = nullptr;

    {
        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loadedObject1);
    }

    BOOST_REQUIRE(loadedObject1 != nullptr);
    BOOST_REQUIRE(loadedObject1->next != nullptr);

    auto loadedObject2 = loadedObject1->next;

    BOOST_REQUIRE(loadedObject1->next == loadedObject2);
    BOOST_REQUIRE(loadedObject2->next == loadedObject1);
    BOOST_REQUIRE(loadedObject1->value == savedObject1->value);
    BOOST_REQUIRE(loadedObject2->value == savedObject2->value);

    delete savedObject1;
    delete savedObject2;
    delete loadedObject1;
    delete loadedObject2;
}

BOOST_AUTO_TEST_SUITE_END()