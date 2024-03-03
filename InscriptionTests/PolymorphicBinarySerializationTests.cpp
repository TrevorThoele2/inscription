#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include <TestFramework/DataGeneration.h>

#include <Inscription/OutputBinaryScribe.h>
#include <Inscription/InputBinaryScribe.h>
#include <Inscription/InscripterBase.h>
#include <Inscription/BinaryConvenience.h>

class Base
{
public:
    Base(int baseValue = 0);
    virtual ~Base() = 0;

    int BaseValue() const;
private:
    int baseValue;
private:
    INSCRIPTION_ACCESS;
};

Base::Base(int baseValue) : baseValue(baseValue)
{}

Base::~Base()
{}

int Base::BaseValue() const
{
    return baseValue;
}

class Derived : public Base
{
public:
    Derived(int baseValue = 0, const std::string& derivedValue = "");

    const std::string& DerivedValue() const;
private:
    std::string derivedValue;
private:
    INSCRIPTION_ACCESS;
};

Derived::Derived(int baseValue, const std::string& derivedValue) : Base(baseValue), derivedValue(derivedValue)
{}

const std::string& Derived::DerivedValue() const
{
    return derivedValue;
}

class BaseRequiredConstruction
{
public:
    BaseRequiredConstruction(int baseValue = 0);
    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(BaseRequiredConstruction);
    virtual ~BaseRequiredConstruction() = 0;

    int BaseValue() const;
private:
    int baseValue;
private:
    INSCRIPTION_ACCESS;
};

BaseRequiredConstruction::BaseRequiredConstruction(int baseValue) : baseValue(baseValue)
{}

INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(BaseRequiredConstruction) : INSCRIPTION_TABLE_GET_MEM(baseValue)
{}

BaseRequiredConstruction::~BaseRequiredConstruction()
{}

int BaseRequiredConstruction::BaseValue() const
{
    return baseValue;
}

class DerivedRequiredConstruction : public BaseRequiredConstruction
{
public:
    DerivedRequiredConstruction(int baseValue, const std::string& derivedValue);
    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(DerivedRequiredConstruction);

    const std::string& DerivedValue() const;
private:
    std::string derivedValue;
private:
    INSCRIPTION_ACCESS;
};

DerivedRequiredConstruction::DerivedRequiredConstruction(int baseValue, const std::string& derivedValue) :
    BaseRequiredConstruction(baseValue), derivedValue(derivedValue)
{}

INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(DerivedRequiredConstruction) :
    INSCRIPTION_TABLE_GET_BASE(BaseRequiredConstruction), INSCRIPTION_TABLE_GET_MEM(derivedValue)
{}

const std::string& DerivedRequiredConstruction::DerivedValue() const
{
    return derivedValue;
}

BOOST_AUTO_TEST_SUITE(PolymorphicBinarySerializationTests)

::TestFramework::DataGeneration dataGeneration;

BOOST_AUTO_TEST_CASE(PolymorphicOwningPointer_SavesAndLoads)
{
    ::Inscription::Scribe::RegisterType<Base>();
    ::Inscription::Scribe::RegisterType<Derived>();

    Base* owning = dataGeneration.Generator<Derived>().RandomHeap();
    Derived* castedOwning = dynamic_cast<Derived*>(owning);

    {
        ::Inscription::OutputBinaryScribe outputScribe("Test.dat", "testing", 0);
        outputScribe.SaveOwningPointer(owning);
    }

    Base* n_owning;

    {
        ::Inscription::InputBinaryScribe inputScribe("Test.dat", "testing");
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
    ::Inscription::Scribe::RegisterType<Base>();
    ::Inscription::Scribe::RegisterType<Derived>();

    auto owning = dataGeneration.Generator<Derived>().RandomStackGroup(2);
    auto unowning = std::vector<Base*>{ &owning[0], &owning[1] };

    {
        ::Inscription::OutputBinaryScribe outputScribe("Test.dat", "testing", 0);
        outputScribe.Save(owning[0]);
        outputScribe.SaveUnowningPointer(unowning[0]);

        outputScribe.SaveUnowningPointer(unowning[1]);
        outputScribe.Save(owning[1]);
    }

    std::vector<Derived> n_owning(2, Derived());
    std::vector<Base*> n_unowning(2, static_cast<Derived*>(nullptr));

    {
        ::Inscription::InputBinaryScribe inputScribe("Test.dat", "testing");
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
    ::Inscription::Scribe::RegisterType<Base>();
    ::Inscription::Scribe::RegisterType<Derived>();

    std::vector<Base*> owning;
    std::vector<Derived*> castedOwning;
    std::vector<Base*> unowning;
    {
        castedOwning = dataGeneration.Generator<Derived>().RandomHeadGroup(2);
        owning.assign(castedOwning.begin(), castedOwning.end());
        unowning = owning;
    }

    {
        ::Inscription::OutputBinaryScribe outputScribe("Test.dat", "testing", 0);
        outputScribe.SaveOwningPointer(owning[0]);
        outputScribe.SaveUnowningPointer(unowning[0]);

        outputScribe.SaveUnowningPointer(unowning[1]);
        outputScribe.SaveOwningPointer(owning[1]);
    }

    std::vector<Base*> n_owning(2, static_cast<Base*>(nullptr));
    std::vector<Base*> n_unowning(2, static_cast<Base*>(nullptr));

    {
        ::Inscription::InputBinaryScribe inputScribe("Test.dat", "testing");
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
    ::Inscription::Scribe::RegisterType<BaseRequiredConstruction>();
    ::Inscription::Scribe::RegisterType<DerivedRequiredConstruction>();

    BaseRequiredConstruction* owning = dataGeneration
        .Generator<DerivedRequiredConstruction>().RandomHeap<int, std::string>();
    DerivedRequiredConstruction* castedOwning = dynamic_cast<DerivedRequiredConstruction*>(owning);

    {
        ::Inscription::OutputBinaryScribe outputScribe("Test.dat", "testing", 0);
        outputScribe.SaveOwningPointer(owning);
    }

    BaseRequiredConstruction* n_owning = nullptr;

    {
        ::Inscription::InputBinaryScribe inputScribe("Test.dat", "testing");
        inputScribe.LoadOwningPointer(n_owning);
    }

    BOOST_REQUIRE(n_owning != nullptr);

    DerivedRequiredConstruction* n_castedOwning = dynamic_cast<DerivedRequiredConstruction*>(n_owning);

    auto test = n_castedOwning->DerivedValue() == castedOwning->DerivedValue();

    BOOST_REQUIRE(n_castedOwning->BaseValue() == owning->BaseValue());
    BOOST_REQUIRE(n_castedOwning->DerivedValue() == castedOwning->DerivedValue());

    delete owning;
    delete n_owning;
}

BOOST_AUTO_TEST_SUITE_END()

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Base)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_TABLE
        {
            INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;
            INSCRIPTION_TABLE_ADD(baseValue);
            INSCRIPTION_INSCRIPTER_RETURN_TABLE;
        };
    };

    INSCRIPTION_INSCRIPTER_DECLARE(::Derived)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_TABLE
        {
            INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;
            INSCRIPTION_TABLE_ADD_BASE(::Base);
            INSCRIPTION_TABLE_ADD(derivedValue);
            INSCRIPTION_INSCRIPTER_RETURN_TABLE;
        };

        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Derived, "Derived")

    INSCRIPTION_INSCRIPTER_DECLARE(::BaseRequiredConstruction)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_TABLE
        {
            INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;
            INSCRIPTION_TABLE_ADD(baseValue);
            INSCRIPTION_INSCRIPTER_RETURN_TABLE;
        };
    };

    INSCRIPTION_INSCRIPTER_DECLARE(::DerivedRequiredConstruction)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_TABLE
        {
            INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;
            INSCRIPTION_TABLE_ADD_BASE(::BaseRequiredConstruction);
            INSCRIPTION_TABLE_ADD(derivedValue);
            INSCRIPTION_INSCRIPTER_RETURN_TABLE;
        };

        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(
        ::DerivedRequiredConstruction, "DerivedRequiredConstruction")
}