#include <boost/test/unit_test.hpp>

#include <Inscription/MemoryScribe.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/PointerScribe.h>
#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"

class BinaryPolymorphicFixture : public BinaryFixture
{
public:
    BinaryPolymorphicFixture()
    {
        typeRegistrationContext.RegisterType<Base>();
        typeRegistrationContext.RegisterType<Derived>();
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

    class UnregisteredBase
    {
    public:
        virtual ~UnregisteredBase() = 0;
    };

    class UnregisteredDerived : public UnregisteredBase
    {};
};

BinaryPolymorphicFixture::Base::~Base()
{}

BinaryPolymorphicFixture::UnregisteredBase::~UnregisteredBase()
{}

namespace Inscription
{
    template<>
    class Scribe<::BinaryPolymorphicFixture::Base, BinaryArchive> : public
        CompositeScribe<::BinaryPolymorphicFixture::Base, BinaryArchive>
    {
    public:
        static void ScrivenImplementation(ObjectT& object, ArchiveT& archive);
    };

    template<>
    class Scribe<::BinaryPolymorphicFixture::Derived, BinaryArchive> : public
        CompositeScribe<::BinaryPolymorphicFixture::Derived, BinaryArchive>
    {
    public:
        static void ScrivenImplementation(ObjectT& object, ArchiveT& archive);
        static const ClassNameResolver classNameResolver;
    };

    template<>
    class Scribe<::BinaryPolymorphicFixture::UnregisteredBase, BinaryArchive> : public
        CompositeScribe<::BinaryPolymorphicFixture::UnregisteredBase, BinaryArchive>
    {
    public:
        static void ScrivenImplementation(ObjectT& object, ArchiveT& archive)
        {}
    };

    template<>
    class Scribe<::BinaryPolymorphicFixture::UnregisteredDerived, BinaryArchive> : public
        CompositeScribe<::BinaryPolymorphicFixture::UnregisteredDerived, BinaryArchive>
    {
    public:
        static void ScrivenImplementation(ObjectT& object, ArchiveT& archive)
        {}
    };
}

BOOST_FIXTURE_TEST_SUITE(BinaryPolymorphicTests, BinaryPolymorphicFixture)

BOOST_AUTO_TEST_CASE(PolymorphicBaseNullPointer_SavesAndLoads)
{
    Base* saved = nullptr;

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

    BOOST_REQUIRE(loadedCasted == nullptr);
}

BOOST_AUTO_TEST_CASE(PolymorphicDerivedNullPointer_SavesAndLoads)
{
    Derived* saved = nullptr;

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(saved);
    }

    Derived* loaded = nullptr;

    {
        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loaded);
    }

    BOOST_REQUIRE(loaded == nullptr);
}

BOOST_AUTO_TEST_CASE(PolymorphicPointer_SavesAndLoads)
{
    Base* saved = dataGeneration.Generator<Derived>().RandomHeap();
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
    BOOST_REQUIRE(loadedCasted->BaseValue() == savedCasted->BaseValue());
    BOOST_REQUIRE(loadedCasted->DerivedValue() == savedCasted->DerivedValue());

    delete saved;
    delete loaded;
}

BOOST_AUTO_TEST_CASE(UnregisteredType_ThrowsExceptionOnSave)
{
    UnregisteredBase* saved = dataGeneration.Generator<UnregisteredDerived>().RandomHeap();

    auto outputArchive = CreateRegistered<OutputArchive>();

    BOOST_REQUIRE_THROW(outputArchive(saved, ::Inscription::Pointer::Owning), ::Inscription::PolymorphicTypeNotFound);

    delete saved;
}

BOOST_AUTO_TEST_SUITE_END()

namespace Inscription
{
    void Scribe<::BinaryPolymorphicFixture::Base, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object.baseValue);
    }

    void Scribe<::BinaryPolymorphicFixture::Derived, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::BinaryPolymorphicFixture::Base>(object, archive);
        archive(object.derivedValue);
    }

    const Scribe<::BinaryPolymorphicFixture::Derived, BinaryArchive>::ClassNameResolver
        Scribe<::BinaryPolymorphicFixture::Derived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("CustomConstructionDerived");
}