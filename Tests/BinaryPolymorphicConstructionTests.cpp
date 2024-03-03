#include <catch.hpp>
#include <utility>

#include <Inscription/CompositeScribeCategory.h>
#include <Inscription/TableScribeCategory.h>
#include <Inscription/NumericScribe.h>

#include <Inscription/MemoryScribe.h>
#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"

class BinaryPolymorphicConstructionFixture : public BinaryFixture
{
public:
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
        void Scriven(ObjectT& object, BinaryArchive& archive);
    };

    template<class Archive>
    struct ScribeTraits<BinaryPolymorphicConstructionFixture::Base, Archive> final
    {
        using Category = CompositeScribeCategory<BinaryPolymorphicConstructionFixture::Base>;
    };

    template<>
    class Scribe<BinaryPolymorphicConstructionFixture::Derived> final
    {
    public:
        using ObjectT = BinaryPolymorphicConstructionFixture::Derived;
    public:
        static void Construct(ObjectT* storage, BinaryArchive& archive);

        static Type OutputType(const BinaryArchive& archive);
    public:
        void Scriven(ObjectT& object, BinaryArchive& archive);
    };

    template<class Archive>
    struct ScribeTraits<BinaryPolymorphicConstructionFixture::Derived, Archive> final
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

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loaded construction-overidden pointer through base pointer")
        {
            Base* loaded = nullptr;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loaded);
            }

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
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(savedUnowned);
            outputArchive(savedOwned);
        }

        WHEN("multiple polymorphic pointers loaded")
        {
            Base* loadedOwned = nullptr;
            Base* loadedUnowned = nullptr;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loadedUnowned);
                inputArchive(loadedOwned);
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
    void Scribe<::BinaryPolymorphicConstructionFixture::Base>::Scriven(ObjectT& object, BinaryArchive& archive)
    {
        archive(object.baseValue);
    }

    void Scribe<BinaryPolymorphicConstructionFixture::Derived>::Construct(ObjectT* storage, BinaryArchive& archive)
    {
        int baseValue;
        archive(baseValue);

        std::string derivedValue;
        archive(derivedValue);

        new (storage) ObjectT(baseValue, derivedValue);
    }

    Type Scribe<BinaryPolymorphicConstructionFixture::Derived>::OutputType(const BinaryArchive& archive)
    {
        return "BinaryPolymorphicConstructionDerived";
    }

    void Scribe<BinaryPolymorphicConstructionFixture::Derived>::Scriven(ObjectT& object, BinaryArchive& archive)
    {
        BaseScriven<::BinaryPolymorphicConstructionFixture::Base>(object, archive);
        archive(object.derivedValue);
    }
}