#include <boost/test/unit_test.hpp>

#include <Inscription/ObjectTracker.h>

#include "BinaryFixture.h"

class ObjectTrackingFixture : public GeneralFixture
{
public:
    using TestedObject = ::Inscription::ObjectTracker;
    TestedObject testedObject;
public:
    class Simple
    {
    public:
        int value = 0;

        Simple() = default;
        Simple(int value) : value(value)
        {}
    };

    class Base
    {
    public:
        int baseValue;
    public:
        Base(int baseValue) : baseValue(baseValue)
        {}

        virtual ~Base() = 0;
    };

    class Derived : public Base
    {
    public:
        std::string derivedValue;
    public:
        Derived(int baseValue, const std::string& derivedValue) :
            Base(baseValue), derivedValue(derivedValue)
        {}
    };
};

ObjectTrackingFixture::Base::~Base()
{}

BOOST_FIXTURE_TEST_SUITE(ObjectTrackingTests, ObjectTrackingFixture)

BOOST_AUTO_TEST_CASE(Lookahead_Materializes)
{
    auto simple = dataGeneration.Generator<Simple>().RandomStack<int>();

    auto id = testedObject.CreateLookahead(sizeof(Simple));

    BOOST_REQUIRE(id.IsValid());

    auto storage = testedObject.LookaheadStorage(id.Get());
    new (storage) Simple(simple);
    auto found = testedObject.FindEntryID(storage);
    BOOST_REQUIRE(!found.IsValid());

    testedObject.MaterializeLookahead(id.Get());
    found = testedObject.FindEntryID(storage);
    BOOST_REQUIRE(found.IsValid());

    ::Inscription::DestroyStorage(reinterpret_cast<std::uint8_t*>(storage));
}

BOOST_AUTO_TEST_SUITE_END()