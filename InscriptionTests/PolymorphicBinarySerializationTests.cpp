#include <boost/test/unit_test.hpp>

#include <Inscription/Memory.h>

#include "BinaryFixture.h"

class PolymorphicBinarySerializationTestsFixture : public BinaryFixture
{
public:
    PolymorphicBinarySerializationTestsFixture()
    {
        typeRegistrationContext.RegisterType<Base>();

        typeRegistrationContext.RegisterType<Derived>();
        typeRegistrationContext.RegisterType<DerivedRequiredConstruction>();
        typeRegistrationContext.RegisterType<DerivedComposite>();
    }

    class Base
    {
    public:
        Base(int baseValue = 0) : baseValue(baseValue)
        {}

        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(Base) :
            INSCRIPTION_TABLE_GET_MEM(baseValue)
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

    class DerivedRequiredConstruction : public Base
    {
    public:
        DerivedRequiredConstruction(int baseValue, const std::string& derivedValue) :
            Base(baseValue), derivedValue(derivedValue)
        {}

        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(DerivedRequiredConstruction) :
            INSCRIPTION_TABLE_GET_BASE(Base), INSCRIPTION_TABLE_GET_MEM(derivedValue)
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

    class DerivedComposite : public Base
    {
    public:
        DerivedComposite(int baseValue, std::unique_ptr<Base>&& ptr) :
            Base(baseValue), ptr(std::move(ptr))
        {}

        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(DerivedComposite) :
            INSCRIPTION_TABLE_GET_BASE(Base), INSCRIPTION_TABLE_GET_MEM(ptr)
        {}

        const std::unique_ptr<Base>& Ptr() const
        {
            return ptr;
        }
    private:
        std::unique_ptr<Base> ptr;
    private:
        INSCRIPTION_ACCESS;
    };
};

PolymorphicBinarySerializationTestsFixture::Base::~Base()
{}

BOOST_FIXTURE_TEST_SUITE(PolymorphicBinarySerializationTests, PolymorphicBinarySerializationTestsFixture)

BOOST_AUTO_TEST_CASE(PolymorphicOwningPointer_SavesAndLoads)
{
    Base* owning = dataGeneration.Generator<Derived>().RandomHeap();
    Derived* castedOwning = dynamic_cast<Derived*>(owning);

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.SaveOwningPointer(owning);
    }

    Base* n_owning;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.LoadOwningPointer(n_owning);
    }

    Derived* n_castedOwning = dynamic_cast<Derived*>(n_owning);

    BOOST_REQUIRE(n_castedOwning != nullptr);
    BOOST_REQUIRE(n_castedOwning->BaseValue() == castedOwning->BaseValue());
    BOOST_REQUIRE(n_castedOwning->DerivedValue() == castedOwning->DerivedValue());

    delete owning;
    delete n_owning;
}

BOOST_AUTO_TEST_CASE(PolymorphicUnowningPointer_SavesAndLoads)
{
    auto owning = dataGeneration.Generator<Derived>().RandomStackGroup(2);
    auto unowning = std::vector<Base*>{ &owning[0], &owning[1] };

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.Save(owning[0]);
        outputScribe.SaveUnowningPointer(unowning[0]);

        outputScribe.SaveUnowningPointer(unowning[1]);
        outputScribe.Save(owning[1]);
    }

    std::vector<Derived> n_owning(2, Derived());
    std::vector<Base*> n_unowning(2, static_cast<Derived*>(nullptr));

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.Load(n_owning[0]);
        inputScribe.LoadUnowningPointer(n_unowning[0]);

        inputScribe.LoadUnowningPointer(n_unowning[1]);
        inputScribe.Load(n_owning[1]);
    }

    std::vector<Derived*> n_castedUnowning{
        dynamic_cast<Derived*>(n_unowning[0]),
        dynamic_cast<Derived*>(n_unowning[1]) };

    BOOST_REQUIRE(n_castedUnowning[0]->BaseValue() == owning[0].BaseValue());
    BOOST_REQUIRE(n_castedUnowning[0]->DerivedValue() == owning[0].DerivedValue());

    BOOST_REQUIRE(n_castedUnowning[1]->BaseValue() == owning[1].BaseValue());
    BOOST_REQUIRE(n_castedUnowning[1]->DerivedValue() == owning[1].DerivedValue());
}

BOOST_AUTO_TEST_CASE(PolymorphicOwningAndUnowningPointer_SavesAndLoads)
{
    std::vector<Base*> owning;
    std::vector<Derived*> castedOwning;
    std::vector<Base*> unowning;
    {
        castedOwning = dataGeneration.Generator<Derived>().RandomHeapGroup(2);
        owning.assign(castedOwning.begin(), castedOwning.end());
        unowning = owning;
    }

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.SaveOwningPointer(owning[0]);
        outputScribe.SaveUnowningPointer(unowning[0]);

        outputScribe.SaveUnowningPointer(unowning[1]);
        outputScribe.SaveOwningPointer(owning[1]);
    }

    std::vector<Base*> n_owning(2, static_cast<Base*>(nullptr));
    std::vector<Base*> n_unowning(2, static_cast<Base*>(nullptr));

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.LoadOwningPointer(n_owning[0]);
        inputScribe.LoadUnowningPointer(n_unowning[0]);

        inputScribe.LoadUnowningPointer(n_unowning[1]);
        inputScribe.LoadOwningPointer(n_owning[1]);
    }

    std::vector<Derived*> n_castedOwning = {
        dynamic_cast<Derived*>(n_owning[0]),
        dynamic_cast<Derived*>(n_owning[1]) };

    std::vector<Derived*> n_castedUnowning = {
        dynamic_cast<Derived*>(n_unowning[0]),
        dynamic_cast<Derived*>(n_unowning[1]) };

    BOOST_REQUIRE(n_castedOwning[0] != nullptr);
    BOOST_REQUIRE(n_castedOwning[0]->BaseValue() == castedOwning[0]->BaseValue());
    BOOST_REQUIRE(n_castedOwning[0]->DerivedValue() == castedOwning[0]->DerivedValue());
    BOOST_REQUIRE(n_castedUnowning[0] != nullptr);
    BOOST_REQUIRE(n_unowning[0] == n_owning[0]);
    BOOST_REQUIRE(n_castedUnowning[0]->BaseValue() == castedOwning[0]->BaseValue());
    BOOST_REQUIRE(n_castedUnowning[0]->DerivedValue() == castedOwning[0]->DerivedValue());

    BOOST_REQUIRE(n_castedOwning[1] != nullptr);
    BOOST_REQUIRE(n_castedOwning[1]->BaseValue() == castedOwning[1]->BaseValue());
    BOOST_REQUIRE(n_castedOwning[1]->DerivedValue() == castedOwning[1]->DerivedValue());
    BOOST_REQUIRE(n_castedUnowning[1] != nullptr);
    BOOST_REQUIRE(n_unowning[1] == n_owning[1]);
    BOOST_REQUIRE(n_castedUnowning[1]->BaseValue() == castedOwning[1]->BaseValue());
    BOOST_REQUIRE(n_castedUnowning[1]->DerivedValue() == castedOwning[1]->DerivedValue());

    delete owning[0];
    delete n_owning[0];
    delete owning[1];
    delete n_owning[1];
}

BOOST_AUTO_TEST_CASE(PolymorphicRequiredConstruction_ConstructsCorrectly)
{
    Base* owning = dataGeneration
        .Generator<DerivedRequiredConstruction>().RandomHeap<int, std::string>();
    DerivedRequiredConstruction* castedOwning = dynamic_cast<DerivedRequiredConstruction*>(owning);

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.SaveOwningPointer(owning);
    }

    Base* n_owning = nullptr;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.LoadOwningPointer(n_owning);
    }

    BOOST_REQUIRE(n_owning != nullptr);

    DerivedRequiredConstruction* n_castedOwning = dynamic_cast<DerivedRequiredConstruction*>(n_owning);

    BOOST_REQUIRE(n_castedOwning->BaseValue() == owning->BaseValue());
    BOOST_REQUIRE(n_castedOwning->DerivedValue() == castedOwning->DerivedValue());

    delete owning;
    delete n_owning;
}

BOOST_AUTO_TEST_CASE(PolymorphicComposite_Constructs)
{
    auto inner = dataGeneration.Generator<Derived>().RandomHeap<int, std::string>();
    Base* owning = new DerivedComposite(dataGeneration.Generator<int>().Random(), std::unique_ptr<Base>(inner));
    DerivedComposite* castedOwning = dynamic_cast<DerivedComposite*>(owning);

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.SaveOwningPointer(owning);
    }

    Base* n_owning = nullptr;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
        inputScribe.LoadOwningPointer(n_owning);
    }

    BOOST_REQUIRE(n_owning != nullptr);

    DerivedComposite* n_castedOwning = dynamic_cast<DerivedComposite*>(n_owning);
    Derived* n_castedPtr = dynamic_cast<Derived*>(n_castedOwning->Ptr().get());

    BOOST_REQUIRE(n_castedOwning->BaseValue() == owning->BaseValue());
    BOOST_REQUIRE(n_castedOwning->Ptr()->BaseValue() == castedOwning->Ptr()->BaseValue());
    BOOST_REQUIRE(n_castedPtr->DerivedValue() == inner->DerivedValue());

    delete owning;
    delete n_owning;
}

BOOST_AUTO_TEST_SUITE_END()

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::PolymorphicBinarySerializationTestsFixture::Base)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_TABLE
        {
            INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;
            INSCRIPTION_TABLE_ADD(baseValue);
            INSCRIPTION_INSCRIPTER_RETURN_TABLE;
        };
    };

    INSCRIPTION_INSCRIPTER_DECLARE(::PolymorphicBinarySerializationTestsFixture::Derived)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_TABLE
        {
            INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;
            INSCRIPTION_TABLE_ADD_BASE(::PolymorphicBinarySerializationTestsFixture::Base);
            INSCRIPTION_TABLE_ADD(derivedValue);
            INSCRIPTION_INSCRIPTER_RETURN_TABLE;
        };

        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(
        ::PolymorphicBinarySerializationTestsFixture::Derived, "Derived");

    INSCRIPTION_INSCRIPTER_DECLARE(::PolymorphicBinarySerializationTestsFixture::DerivedRequiredConstruction)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_TABLE
        {
            INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;
            INSCRIPTION_TABLE_ADD_BASE(::PolymorphicBinarySerializationTestsFixture::Base);
            INSCRIPTION_TABLE_ADD(derivedValue);
            INSCRIPTION_INSCRIPTER_RETURN_TABLE;
        };

        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(
        ::PolymorphicBinarySerializationTestsFixture::DerivedRequiredConstruction, "DerivedRequiredConstruction");

    INSCRIPTION_INSCRIPTER_DECLARE(::PolymorphicBinarySerializationTestsFixture::DerivedComposite)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_TABLE
        {
            INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;
            INSCRIPTION_TABLE_ADD_BASE(::PolymorphicBinarySerializationTestsFixture::Base);
            INSCRIPTION_TABLE_ADD(ptr);
            INSCRIPTION_INSCRIPTER_RETURN_TABLE;
        };

        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(
        ::PolymorphicBinarySerializationTestsFixture::DerivedComposite, "DerivedComposite");
}