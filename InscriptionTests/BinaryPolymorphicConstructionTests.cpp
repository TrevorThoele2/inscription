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
    public:
        static void ScrivenImplementation(ObjectT& object, ArchiveT& archive);
    };

    template<>
    class Scribe<::BinaryPolymorphicConstructionFixture::Derived, BinaryArchive> : public
        CompositeScribe<::BinaryPolymorphicConstructionFixture::Derived, BinaryArchive>
    {
    public:
        static void ScrivenImplementation(ObjectT& object, ArchiveT& archive);
        static void Construct(ObjectT* storage, ArchiveT& archive);
        static const ClassNameResolver classNameResolver;
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
        outputArchive(saved, ::Inscription::Pointer::Owning);
    }

    Base* loaded = nullptr;

    {
        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loaded, ::Inscription::Pointer::Owning);
    }

    Derived* loadedCasted = dynamic_cast<Derived*>(loaded);

    BOOST_REQUIRE(loadedCasted != nullptr);
    BOOST_REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
    BOOST_REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);

    delete saved;
    delete loaded;
}

BOOST_AUTO_TEST_SUITE_END()

namespace Inscription
{
    void Scribe<::BinaryPolymorphicConstructionFixture::Base, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object.baseValue);
    }

    void Scribe<::BinaryPolymorphicConstructionFixture::Derived, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::BinaryPolymorphicConstructionFixture::Base>(object, archive);
        archive(object.derivedValue);
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

    const Scribe<::BinaryPolymorphicConstructionFixture::Derived, BinaryArchive>::ClassNameResolver
        Scribe<::BinaryPolymorphicConstructionFixture::Derived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("CustomConstructionDerived");
}