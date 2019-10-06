#include <catch.hpp>
#include <utility>

#include <Inscription/TableScribe.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/PointerScribe.h>

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
    class Scribe<::BinaryPolymorphicConstructionFixture::Base, BinaryArchive> : public
        CompositeScribe<::BinaryPolymorphicConstructionFixture::Base, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };

    template<>
    class Scribe<::BinaryPolymorphicConstructionFixture::Derived, BinaryArchive> : public
        CompositeScribe<::BinaryPolymorphicConstructionFixture::Derived, BinaryArchive>
    {
    public:
        static void Construct(ObjectT* storage, ArchiveT& archive);

        static TypeHandle PrincipleTypeHandle(const ArchiveT& archive);
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
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
    void Scribe<::BinaryPolymorphicConstructionFixture::Base, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object.baseValue);
    }

    void Scribe<::BinaryPolymorphicConstructionFixture::Derived, BinaryArchive>::Construct(
        ObjectT* storage, ArchiveT& archive)
    {
        int baseValue;
        archive(baseValue);

        std::string derivedValue;
        archive(derivedValue);

        new (storage) ObjectT(baseValue, derivedValue);
    }

    TypeHandle Scribe<::BinaryPolymorphicConstructionFixture::Derived, BinaryArchive>::PrincipleTypeHandle(
        const ArchiveT& archive
    ) {
        return "BinaryPolymorphicConstructionDerived";
    }

    void Scribe<::BinaryPolymorphicConstructionFixture::Derived, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive.BaseScriven<::BinaryPolymorphicConstructionFixture::Base>(object);
        archive(object.derivedValue);
    }
}