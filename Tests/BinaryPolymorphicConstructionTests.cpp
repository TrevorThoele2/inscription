#include <catch.hpp>
#include <utility>

#include <Inscription/CompositeScribeCategory.h>
#include <Inscription/TableScribeCategory.h>
#include <Inscription/NumericScribe.h>

#include <Inscription/MemoryScribe.h>
#include <Inscription/StringScribe.h>

#include "GeneralFixture.h"

class BinaryPolymorphicConstructionFixture : public GeneralFixture
{
public:
    Inscription::TypeRegistrationContext<Inscription::Format::Binary> typeRegistrationContext;

    BinaryPolymorphicConstructionFixture()
    {
        typeRegistrationContext.RegisterType<Base>();
        typeRegistrationContext.RegisterType<Derived>();
    }

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
        Derived(int baseValue, std::string derivedValue) :
            Base(baseValue), derivedValue(std::move(derivedValue))
        {}
    };
};

BinaryPolymorphicConstructionFixture::Base::~Base() = default;

namespace Inscription
{
    template<>
    class Scribe<BinaryPolymorphicConstructionFixture::Base> final
    {
    public:
        using ObjectT = BinaryPolymorphicConstructionFixture::Base;
    public:
        void Scriven(ObjectT& object, Format::Binary& format);
    };

    template<class Format>
    struct ScribeTraits<BinaryPolymorphicConstructionFixture::Base, Format> final
    {
        using Category = CompositeScribeCategory<BinaryPolymorphicConstructionFixture::Base>;
    };

    template<>
    class Scribe<BinaryPolymorphicConstructionFixture::Derived> final
    {
    public:
        using ObjectT = BinaryPolymorphicConstructionFixture::Derived;
    public:
        static void Construct(ObjectT* storage, Format::Binary& format);

        static Type OutputType(const Format::Binary& format);
    public:
        void Scriven(ObjectT& object, Format::Binary& format);
    };

    template<class Format>
    struct ScribeTraits<BinaryPolymorphicConstructionFixture::Derived, Format> final
    {
        using Category = CompositeScribeCategory<BinaryPolymorphicConstructionFixture::Derived>;
    };
}

SCENARIO_METHOD(
    BinaryPolymorphicConstructionFixture,
    "loading polymorphic pointer in binary",
    "[binary][pointer][polymorphic]"
) {
    GIVEN("saved construction-overidden pointer through base pointer")
    {
        Base* saved = dataGeneration.RandomHeap<Derived, int, std::string>();
        Derived* savedCasted = dynamic_cast<Derived*>(saved);

        Inscription::Binary::ToFile(saved, "Test.dat", typeRegistrationContext);

        WHEN("loaded construction-overidden pointer through base pointer")
        {
            Base* loaded = nullptr;

            Inscription::Binary::FromFile(loaded, "Test.dat", typeRegistrationContext);

            Derived* loadedCasted = dynamic_cast<Derived*>(loaded);

            THEN("loaded pointer is valid")
            {
                REQUIRE(loadedCasted != nullptr);
                REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
                REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);

                delete saved;
                delete loaded;
            }
        }
    }

    GIVEN("multiple polymorphic pointers saved")
    {
        Base* savedOwned = dataGeneration.RandomHeap<Derived, int, std::string>();
        Derived* savedOwnedCasted = dynamic_cast<Derived*>(savedOwned);

        Base* savedUnowned = savedOwned;
        Derived* savedUnownedCasted = savedOwnedCasted;

        {
            auto file = Inscription::File::OutputBinary("Test.dat");
            auto archive = Inscription::Archive::OutputBinary(file);
            auto format = Inscription::Format::OutputBinary(archive, typeRegistrationContext);

            format(savedUnowned);
            format(savedOwned);
        }

        WHEN("multiple polymorphic pointers loaded")
        {
            Base* loadedOwned = nullptr;
            Base* loadedUnowned = nullptr;

            {
                auto file = Inscription::File::InputBinary("Test.dat");
                auto archive = Inscription::Archive::InputBinary(file);
                auto format = Inscription::Format::InputBinary(archive, typeRegistrationContext);

                format(loadedUnowned);
                format(loadedOwned);
            }

            Derived* loadedOwnedCasted = dynamic_cast<Derived*>(loadedOwned);
            Derived* loadedUnownedCasted = dynamic_cast<Derived*>(loadedUnowned);

            THEN("every pointer is valid")
            {
                REQUIRE(loadedUnownedCasted != nullptr);
                REQUIRE(loadedOwnedCasted != nullptr);
                REQUIRE(loadedOwnedCasted == loadedUnownedCasted);
                REQUIRE(loadedOwnedCasted->baseValue == savedOwnedCasted->baseValue);
                REQUIRE(loadedOwnedCasted->derivedValue == savedOwnedCasted->derivedValue);

                delete savedOwned;
                delete loadedOwned;
            }
        }
    }
}

namespace Inscription
{
    void Scribe<BinaryPolymorphicConstructionFixture::Base>::Scriven(ObjectT& object, Format::Binary& format)
    {
        format(object.baseValue);
    }

    void Scribe<BinaryPolymorphicConstructionFixture::Derived>::Construct(ObjectT* storage, Format::Binary& format)
    {
        int baseValue;
        format(baseValue);

        std::string derivedValue;
        format(derivedValue);

        new (storage) ObjectT(baseValue, derivedValue);
    }

    Type Scribe<BinaryPolymorphicConstructionFixture::Derived>::OutputType(const Format::Binary& format)
    {
        return "BinaryPolymorphicConstructionDerived";
    }

    void Scribe<BinaryPolymorphicConstructionFixture::Derived>::Scriven(ObjectT& object, Format::Binary& format)
    {
        BaseScriven<BinaryPolymorphicConstructionFixture::Base>(object, format);
        format(object.derivedValue);
    }
}