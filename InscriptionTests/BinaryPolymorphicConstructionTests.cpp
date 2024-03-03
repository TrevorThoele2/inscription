#include <boost/test/unit_test.hpp>

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
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
        {
            DoBasicConstruction(storage, archive);
        }
    };

    template<>
    class Scribe<::BinaryPolymorphicConstructionFixture::Derived, BinaryArchive> : public
        CompositeScribe<::BinaryPolymorphicConstructionFixture::Derived, BinaryArchive>
    {
    public:
        static const ClassNameResolver classNameResolver;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };
}

BOOST_FIXTURE_TEST_SUITE(BinaryPolymorphicConstructionTests,
    BinaryPolymorphicConstructionFixture)

BOOST_AUTO_TEST_CASE(ManualConstruction)
{
    Base* saved = dataGeneration.Generator<Derived>().RandomHeap<int, std::string>();
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

    BOOST_REQUIRE(loadedCasted != nullptr);
    BOOST_REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
    BOOST_REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);

    delete saved;
    delete loaded;
}

BOOST_AUTO_TEST_CASE(DoesNotConstruct)
{
    Base* saved = dataGeneration.Generator<Derived>().RandomHeap<int, std::string>();
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

    BOOST_REQUIRE(loaded != nullptr);
    BOOST_REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
    BOOST_REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);

    delete saved;
    delete loaded;
}

BOOST_AUTO_TEST_CASE(Load_CausesOtherToBePopulated)
{
    Base* savedOwned = dataGeneration.Generator<Derived>().RandomHeap<int, std::string>();
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

    BOOST_REQUIRE(loadedUnownedCasted != nullptr);
    BOOST_REQUIRE(loadedOwnedCasted != nullptr);
    BOOST_REQUIRE(loadedOwnedCasted == loadedUnownedCasted);
    BOOST_REQUIRE(loadedOwnedCasted->baseValue == savedOwnedCasted->baseValue);
    BOOST_REQUIRE(loadedOwnedCasted->derivedValue == savedOwnedCasted->derivedValue);

    delete savedOwned;
    delete loadedOwned;
}

BOOST_AUTO_TEST_SUITE_END()

namespace Inscription
{
    void Scribe<::BinaryPolymorphicConstructionFixture::Base, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object.baseValue);
    }

    const Scribe<::BinaryPolymorphicConstructionFixture::Derived, BinaryArchive>::ClassNameResolver
        Scribe<::BinaryPolymorphicConstructionFixture::Derived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("CustomConstructionDerived");

    void Scribe<::BinaryPolymorphicConstructionFixture::Derived, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::BinaryPolymorphicConstructionFixture::Base>(object, archive);
        archive(object.derivedValue);
    }

    void Scribe<::BinaryPolymorphicConstructionFixture::Derived, BinaryArchive>::ConstructImplementation(
        ObjectT* storage, ArchiveT& archive)
    {
        int baseValue;
        archive(baseValue);

        std::string derivedValue;
        archive(derivedValue);

        new (storage) ObjectT(baseValue, derivedValue);
    }
}