#include <boost/test/unit_test.hpp>

#include <Inscription/NumericScribe.h>
#include <Inscription/PointerScribe.h>
#include <Inscription/CompositeScribe.h>

#include "BinaryFixture.h"

class BinaryTestsFixture : public BinaryFixture
{
public:
    BinaryTestsFixture()
    {
        typeRegistrationContext.RegisterType<TestClass>();
    }

    class TestClass
    {
    public:
        TestClass(int value = 0) : value(value)
        {}

        int Value() const
        {
            return value;
        }
    private:
        int value;
    private:
        INSCRIPTION_ACCESS;
    };

    TestClass CreateRandomTestClass()
    {
        return dataGeneration.Generator<TestClass>().RandomStack<int>();
    }

    TestClass* CreateRandomHeapTestClass()
    {
        return dataGeneration.Generator<TestClass>().RandomHeap<int>();
    }
};

namespace Inscription
{
    template<>
    class Scribe<::BinaryTestsFixture::TestClass, BinaryArchive> :
        public CompositeScribe<::BinaryTestsFixture::TestClass, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive)
        {
            archive(object.value);
        }
    };
}

BOOST_FIXTURE_TEST_SUITE(BinaryTests, BinaryTestsFixture)

BOOST_AUTO_TEST_CASE(Integer_SavesAndLoads)
{
    std::int8_t savedInt8 = dataGeneration.Generator<std::int8_t>().Random();
    std::int16_t savedInt16 = dataGeneration.Generator<std::int16_t>().Random();
    std::int32_t savedInt32 = dataGeneration.Generator<std::int32_t>().Random();
    std::int64_t savedInt64 = dataGeneration.Generator<std::int64_t>().Random();

    std::uint8_t savedIint8 = dataGeneration.Generator<std::uint8_t>().Random();
    std::uint16_t savedUint16 = dataGeneration.Generator<std::uint16_t>().Random();
    std::uint32_t savedUint32 = dataGeneration.Generator<std::uint32_t>().Random();
    std::uint64_t savedUint64 = dataGeneration.Generator<std::uint64_t>().Random();

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(savedInt8);
        outputArchive(savedInt16);
        outputArchive(savedInt32);
        outputArchive(savedInt64);

        outputArchive(savedIint8);
        outputArchive(savedUint16);
        outputArchive(savedUint32);
        outputArchive(savedUint64);
    }

    std::int8_t loadedInt8 = 0;
    std::int16_t loadedInt16 = 0;
    std::int32_t loadedInt32 = 0;
    std::int64_t loadedInt64 = 0;

    std::uint8_t loadedUint8 = 0;
    std::uint16_t loadedUint16 = 0;
    std::uint32_t loadedUint32 = 0;
    std::uint64_t loadedUint64 = 0;

    {
        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loadedInt8);
        inputArchive(loadedInt16);
        inputArchive(loadedInt32);
        inputArchive(loadedInt64);

        inputArchive(loadedUint8);
        inputArchive(loadedUint16);
        inputArchive(loadedUint32);
        inputArchive(loadedUint64);
    }

    BOOST_REQUIRE(loadedInt8 == savedInt8);
    BOOST_REQUIRE(loadedInt16 == savedInt16);
    BOOST_REQUIRE(loadedInt32 == savedInt32);
    BOOST_REQUIRE(loadedInt64 == savedInt64);

    BOOST_REQUIRE(loadedUint8 == savedIint8);
    BOOST_REQUIRE(loadedUint16 == savedUint16);
    BOOST_REQUIRE(loadedUint32 == savedUint32);
    BOOST_REQUIRE(loadedUint64 == savedUint64);
}

BOOST_AUTO_TEST_CASE(FloatingPoint_SavesAndLoads)
{
    float savedFloat = dataGeneration.Generator<float>().Random();
    double savedDouble = dataGeneration.Generator<double>().Random();

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(savedFloat);
        outputArchive(savedDouble);
    }

    float loadedFloat = 0.0f;
    double loadedDouble = 0.0;

    {
        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loadedFloat);
        inputArchive(loadedDouble);
    }

    BOOST_REQUIRE(loadedFloat == savedFloat);
    BOOST_REQUIRE(loadedDouble == savedDouble);
}

BOOST_AUTO_TEST_CASE(Class_SavesAndLoads)
{
    auto savedTestClass = CreateRandomTestClass();

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(savedTestClass);
    }

    TestClass loadedTestClass(0);

    {
        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loadedTestClass);
    }

    BOOST_REQUIRE(loadedTestClass.Value() == savedTestClass.Value());
}

BOOST_AUTO_TEST_CASE(Pointer_SavesAndLoads)
{
    auto saved = CreateRandomHeapTestClass();

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(saved);
    }

    TestClass* loaded = nullptr;

    {
        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loaded);
    }

    BOOST_REQUIRE(loaded->Value() == saved->Value());

    delete saved;
    delete loaded;
}

BOOST_AUTO_TEST_SUITE_END()