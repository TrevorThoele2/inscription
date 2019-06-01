#include <boost/test/unit_test.hpp>

#include "BinaryFixture.h"

#include <Inscription/ContainerSize.h>

class ListPolymorphicBinarySerializationTestsFixture : public BinaryFixture
{
public:
    ListPolymorphicBinarySerializationTestsFixture()
    {
        typeRegistrationContext.RegisterType<Base>();
        typeRegistrationContext.RegisterType<Derived>();

        typeRegistrationContext.RegisterType<BaseRequiredConstruction>();
        typeRegistrationContext.RegisterType<DerivedRequiredConstruction>();
    }

    class Base
    {
    public:
        Base(int baseValue = 0) : baseValue(baseValue)
        {}

        virtual ~Base() = 0;

        int BaseValue() const
        {
            return baseValue;
        }
    private:
        int baseValue;
    private:
        INSCRIPTION_ACCESS;
    };

    class Derived : public Base
    {
    public:
        Derived(int baseValue = 0, const std::string& derivedValue = "") :
            Base(baseValue), derivedValue(derivedValue)
        {}

        const std::string& DerivedValue() const
        {
            return derivedValue;
        }
    private:
        std::string derivedValue;
    private:
        INSCRIPTION_ACCESS;
    };

    class BaseRequiredConstruction
    {
    public:
        BaseRequiredConstruction(int baseValue = 0) : baseValue(baseValue)
        {}

        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(BaseRequiredConstruction) :
            INSCRIPTION_TABLE_GET_MEM(baseValue)
        {}

        virtual ~BaseRequiredConstruction() = 0;

        int BaseValue() const
        {
            return baseValue;
        }
    private:
        int baseValue;
    private:
        INSCRIPTION_ACCESS;
    };

    class DerivedRequiredConstruction : public BaseRequiredConstruction
    {
    public:
        DerivedRequiredConstruction(int baseValue, const std::string& derivedValue) :
            BaseRequiredConstruction(baseValue), derivedValue(derivedValue)
        {}

        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(DerivedRequiredConstruction) :
            INSCRIPTION_TABLE_GET_BASE(BaseRequiredConstruction), INSCRIPTION_TABLE_GET_MEM(derivedValue)
        {}

        const std::string& DerivedValue() const
        {
            return derivedValue;
        }
    private:
        std::string derivedValue;
    private:
        INSCRIPTION_ACCESS;
    };
};

ListPolymorphicBinarySerializationTestsFixture::Base::~Base()
{}

ListPolymorphicBinarySerializationTestsFixture::BaseRequiredConstruction::~BaseRequiredConstruction()
{}

BOOST_FIXTURE_TEST_SUITE(ListPolymorphicBinarySerializationTests, ListPolymorphicBinarySerializationTestsFixture)

BOOST_AUTO_TEST_CASE(PolymorphicOwningPointer_SavesAndLoads)
{
    std::vector<Base*> owning;
    {
        auto startingGroup = dataGeneration.Generator<Derived>().RandomHeapGroup(3);
        for (auto& loop : startingGroup)
            owning.push_back(loop);
    }

    std::vector<Derived*> castedOwning;
    for (auto& loop : owning)
        castedOwning.push_back(dynamic_cast<Derived*>(loop));

    {
        auto outputScribe = CreateRegistered<OutputScribe>();

        ::Inscription::ContainerSize containerSize(owning.size());
        outputScribe.Save(containerSize);
        for (auto& loop : owning)
            outputScribe.SaveOwningPointer(loop);
    }

    std::vector<Base*> n_owning;

    {
        auto inputScribe = CreateRegistered<InputScribe>();

        ::Inscription::ContainerSize containerSize;
        inputScribe.Load(containerSize);
        while (containerSize-- > 0)
        {
            Base* ptr;
            inputScribe.LoadOwningPointer(ptr);
            n_owning.push_back(ptr);
        }
    }

    std::vector<Derived*> n_castedOwning;
    for (auto& loop : n_owning)
        n_castedOwning.push_back(dynamic_cast<Derived*>(loop));

    BOOST_REQUIRE(!n_owning.empty());
    BOOST_REQUIRE(n_owning.size() == owning.size());
    for (size_t i = 0; i < owning.size(); ++i)
    {
        auto owningElement = owning[i];
        auto n_owningElement = n_owning[i];
        auto castedOwningElement = castedOwning[i];
        auto n_castedOwningElement = n_castedOwning[i];

        BOOST_REQUIRE(owningElement->BaseValue() == n_owningElement->BaseValue());
        BOOST_REQUIRE(castedOwningElement->DerivedValue() == n_castedOwningElement->DerivedValue());
    }

    for (auto& loop : owning)
        delete loop;
    for (auto& loop : n_owning)
        delete loop;
}

BOOST_AUTO_TEST_SUITE_END()

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::ListPolymorphicBinarySerializationTestsFixture::Base)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_TABLE
        {
            INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;
            INSCRIPTION_TABLE_ADD(baseValue);
            INSCRIPTION_INSCRIPTER_RETURN_TABLE;
        };
    };

    INSCRIPTION_INSCRIPTER_DECLARE(::ListPolymorphicBinarySerializationTestsFixture::Derived)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_TABLE
        {
            INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;
            INSCRIPTION_TABLE_ADD_BASE(::ListPolymorphicBinarySerializationTestsFixture::Base);
            INSCRIPTION_TABLE_ADD(derivedValue);
            INSCRIPTION_INSCRIPTER_RETURN_TABLE;
        };

        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(
        ::ListPolymorphicBinarySerializationTestsFixture::Derived, "Derived")

        INSCRIPTION_INSCRIPTER_DECLARE(::ListPolymorphicBinarySerializationTestsFixture::BaseRequiredConstruction)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_TABLE
        {
            INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;
            INSCRIPTION_TABLE_ADD(baseValue);
            INSCRIPTION_INSCRIPTER_RETURN_TABLE;
        };
    };

    INSCRIPTION_INSCRIPTER_DECLARE(::ListPolymorphicBinarySerializationTestsFixture::DerivedRequiredConstruction)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_TABLE
        {
            INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;
            INSCRIPTION_TABLE_ADD_BASE(::ListPolymorphicBinarySerializationTestsFixture::BaseRequiredConstruction);
            INSCRIPTION_TABLE_ADD(derivedValue);
            INSCRIPTION_INSCRIPTER_RETURN_TABLE;
        };

        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(
        ::ListPolymorphicBinarySerializationTestsFixture::DerivedRequiredConstruction, "DerivedRequiredConstruction")
}