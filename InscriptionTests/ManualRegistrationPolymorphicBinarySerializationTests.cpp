#include <boost/test/unit_test.hpp>

#include "BinaryFixture.h"

class ManualRegistrationPolymorphicBinarySerializationTestsFixture : public BinaryFixture
{
public:
    ManualRegistrationPolymorphicBinarySerializationTestsFixture()
    {
        typeRegistrationContext.RegisterType<Base>();
        typeRegistrationContext.RegisterType<Derived>();

        typeRegistrationContext.RegisterType<BaseRequiredConstruction>();
        typeRegistrationContext.RegisterType<DerivedRequiredConstruction>();
    }

    template<class T>
    T CreateRegistered() const
    {
        static_assert(false, "A generic registered type cannot be created. Use one of the explicit overloads.");
    }

    template<>
    OutputScribe CreateRegistered() const
    {
        OutputScribe created("Test.dat", "testing", 1);

        created.RegisterType<Base>();
        created.RegisterType<Derived>();

        created.RegisterType<BaseRequiredConstruction>();
        created.RegisterType<DerivedRequiredConstruction>();

        return created;
    }

    template<>
    InputScribe CreateRegistered() const
    {
        InputScribe created("Test.dat", "testing");

        created.RegisterType<Base>();
        created.RegisterType<Derived>();

        created.RegisterType<BaseRequiredConstruction>();
        created.RegisterType<DerivedRequiredConstruction>();

        return created;
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

ManualRegistrationPolymorphicBinarySerializationTestsFixture::Base::~Base()
{}

ManualRegistrationPolymorphicBinarySerializationTestsFixture::BaseRequiredConstruction::~BaseRequiredConstruction()
{}

BOOST_FIXTURE_TEST_SUITE(ManualRegistrationPolymorphicBinarySerializationTests, ManualRegistrationPolymorphicBinarySerializationTestsFixture)

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
    BaseRequiredConstruction* owning = dataGeneration
        .Generator<DerivedRequiredConstruction>().RandomHeap<int, std::string>();
    DerivedRequiredConstruction* castedOwning = dynamic_cast<DerivedRequiredConstruction*>(owning);

    {
        auto outputScribe = CreateRegistered<OutputScribe>();
        outputScribe.SaveOwningPointer(owning);
    }

    BaseRequiredConstruction* n_owning = nullptr;

    {
        auto inputScribe = CreateRegistered<InputScribe>();
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
    INSCRIPTION_INSCRIPTER_DECLARE(::ManualRegistrationPolymorphicBinarySerializationTestsFixture::Base)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_TABLE
        {
            INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;
            INSCRIPTION_TABLE_ADD(baseValue);
            INSCRIPTION_INSCRIPTER_RETURN_TABLE;
        };
    };

    INSCRIPTION_INSCRIPTER_DECLARE(::ManualRegistrationPolymorphicBinarySerializationTestsFixture::Derived)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_TABLE
        {
            INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;
            INSCRIPTION_TABLE_ADD_BASE(::ManualRegistrationPolymorphicBinarySerializationTestsFixture::Base);
            INSCRIPTION_TABLE_ADD(derivedValue);
            INSCRIPTION_INSCRIPTER_RETURN_TABLE;
        };

        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(
        ::ManualRegistrationPolymorphicBinarySerializationTestsFixture::Derived, "Derived")

    INSCRIPTION_INSCRIPTER_DECLARE(
        ::ManualRegistrationPolymorphicBinarySerializationTestsFixture::BaseRequiredConstruction)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_TABLE
        {
            INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;
            INSCRIPTION_TABLE_ADD(baseValue);
            INSCRIPTION_INSCRIPTER_RETURN_TABLE;
        };
    };

    INSCRIPTION_INSCRIPTER_DECLARE(
        ::ManualRegistrationPolymorphicBinarySerializationTestsFixture::DerivedRequiredConstruction)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_TABLE
        {
            INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;
            INSCRIPTION_TABLE_ADD_BASE(
                ::ManualRegistrationPolymorphicBinarySerializationTestsFixture::BaseRequiredConstruction);
            INSCRIPTION_TABLE_ADD(derivedValue);
            INSCRIPTION_INSCRIPTER_RETURN_TABLE;
        };

        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };

    INSCRIPTION_BINARY_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(
        ::ManualRegistrationPolymorphicBinarySerializationTestsFixture::DerivedRequiredConstruction,
        "DerivedRequiredConstruction")
}