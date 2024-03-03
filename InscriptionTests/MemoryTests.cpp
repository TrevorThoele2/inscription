#include <boost/test/unit_test.hpp>

#include <Inscription/Memory.h>

#include "BinaryFixture.h"

class MemoryTestsFixture : public BinaryFixture
{
public:
    ::TestFramework::DataGeneration dataGeneration;

    MemoryTestsFixture()
    {
        typeRegistrationContext.RegisterType<Base>();
        typeRegistrationContext.RegisterType<Derived>();
    }

    class Base
    {
    public:
        int baseValue = 0;
    public:
        virtual ~Base() = 0;
    };

    class Derived : public Base
    {
    public:
        std::string derivedValue;
    };
};

MemoryTestsFixture::Base::~Base()
{}

BOOST_FIXTURE_TEST_SUITE(MemoryTests, MemoryTestsFixture)

BOOST_AUTO_TEST_CASE(UniquePtrSavesAndLoads_ThroughBaseClassPointer)
{
    std::unique_ptr<Base> ptr(new Derived());

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(ptr);
    }

    std::unique_ptr<Base> n_ptr;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_ptr);
    }

    Derived* derivedPtr = static_cast<Derived*>(ptr.get());
    Derived* n_derivedPtr = static_cast<Derived*>(n_ptr.get());

    BOOST_REQUIRE(n_ptr.get() != nullptr);
    BOOST_REQUIRE(n_ptr->baseValue == ptr->baseValue);
    BOOST_REQUIRE(n_derivedPtr->derivedValue == derivedPtr->derivedValue);
}

BOOST_AUTO_TEST_SUITE_END()

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::MemoryTestsFixture::Base)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_TABLE
        {
            INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;
            INSCRIPTION_TABLE_ADD(baseValue);
            INSCRIPTION_INSCRIPTER_RETURN_TABLE;
        };
    };

    INSCRIPTION_INSCRIPTER_DECLARE(::MemoryTestsFixture::Derived)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_TABLE
        {
            INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;
            INSCRIPTION_TABLE_ADD_BASE(::MemoryTestsFixture::Base);
            INSCRIPTION_TABLE_ADD(derivedValue);
            INSCRIPTION_INSCRIPTER_RETURN_TABLE;
        };

        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::MemoryTestsFixture::Derived, "Derived")
}