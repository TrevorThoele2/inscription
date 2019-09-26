#include <catch.hpp>

#include <Inscription/TableScribe.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/PointerScribe.h>

#include <Inscription/MemoryScribe.h>
#include <Inscription/StringScribe.h>

#include <Inscription/BaseScriven.h>

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
        Derived(int baseValue, const std::string& derivedValue) :
            Base(baseValue), derivedValue(derivedValue)
        {}
    };
};

BinaryPolymorphicConstructionFixture::Base::~Base()
{}

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

        static ClassName ClassNameResolver(const ArchiveT& archive);
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}

TEST_CASE_METHOD(BinaryPolymorphicConstructionFixture, "binary polymorphic construction")
{
    SECTION("manual construction")
    {
        Base* saved = dataGeneration.RandomHeap<Derived, int, std::string>();
        Derived* savedCasted = dynamic_cast<Derived*>(saved);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        Base* loaded = nullptr;

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);
        }

        Derived* loadedCasted = dynamic_cast<Derived*>(loaded);

        REQUIRE(loadedCasted != nullptr);
        REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
        REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);

        delete saved;
        delete loaded;
    }

    SECTION("does not construct")
    {
        Base* saved = dataGeneration.RandomHeap<Derived, int, std::string>();
        Derived* savedCasted = dynamic_cast<Derived*>(saved);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        Base* loaded = nullptr;

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);
        }

        Derived* loadedCasted = dynamic_cast<Derived*>(loaded);

        REQUIRE(loaded != nullptr);
        REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
        REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);

        delete saved;
        delete loaded;
    }

    SECTION("load causes other to be populated")
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

        Base* loadedOwned = nullptr;
        Base* loadedUnowned = nullptr;

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loadedUnowned);
            inputArchive(loadedOwned);
        }

        Derived* loadedOwnedCasted = dynamic_cast<Derived*>(loadedOwned);
        Derived* loadedUnownedCasted = dynamic_cast<Derived*>(loadedUnowned);

        REQUIRE(loadedUnownedCasted != nullptr);
        REQUIRE(loadedOwnedCasted != nullptr);
        REQUIRE(loadedOwnedCasted == loadedUnownedCasted);
        REQUIRE(loadedOwnedCasted->baseValue == savedOwnedCasted->baseValue);
        REQUIRE(loadedOwnedCasted->derivedValue == savedOwnedCasted->derivedValue);

        delete savedOwned;
        delete loadedOwned;
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

    ClassName Scribe<::BinaryPolymorphicConstructionFixture::Derived, BinaryArchive>::ClassNameResolver(
        const ArchiveT& archive
    ) {
        return "BinaryPolymorphicConstructionDerived";
    }

    void Scribe<::BinaryPolymorphicConstructionFixture::Derived, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::BinaryPolymorphicConstructionFixture::Base>(object, archive);
        archive(object.derivedValue);
    }
}