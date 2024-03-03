#include <boost/test/unit_test.hpp>

#include <Inscription/NumericScribe.h>
#include <Inscription/PointerScribe.h>
#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"

class BinaryPolymorphicManualRegistrationFixture : public BinaryFixture
{
public:
    BinaryPolymorphicManualRegistrationFixture()
    {
        typeRegistrationContext.RegisterType<Base>();
        typeRegistrationContext.RegisterType<Derived>();
    }

    template<class T>
    T CreateRegistered() const
    {
        static_assert(false, "A generic registered type cannot be created. Use one of the explicit overloads.");
    }

    template<>
    OutputArchive CreateRegistered() const
    {
        OutputArchive created("Test.dat", "testing", 1);

        created.RegisterType<Base>();
        created.RegisterType<Derived>();

        return created;
    }

    template<>
    InputArchive CreateRegistered() const
    {
        InputArchive created("Test.dat", "testing");

        created.RegisterType<Base>();
        created.RegisterType<Derived>();

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
};

BinaryPolymorphicManualRegistrationFixture::Base::~Base()
{}

namespace Inscription
{
    template<>
    class Scribe<::BinaryPolymorphicManualRegistrationFixture::Base, BinaryArchive> : public
        CompositeScribe<::BinaryPolymorphicManualRegistrationFixture::Base, BinaryArchive>
    {
    public:
        static void ScrivenImplementation(ObjectT& object, ArchiveT& archive);
    };

    template<>
    class Scribe<::BinaryPolymorphicManualRegistrationFixture::Derived, BinaryArchive> : public
        CompositeScribe<::BinaryPolymorphicManualRegistrationFixture::Derived, BinaryArchive>
    {
    public:
        static void ScrivenImplementation(ObjectT& object, ArchiveT& archive);
        static const ClassNameResolver classNameResolver;
    };
}

BOOST_FIXTURE_TEST_SUITE(BinaryPolymorphicManualRegistrationTests, BinaryPolymorphicManualRegistrationFixture)

BOOST_AUTO_TEST_CASE(PolymorphicPointer_SavesAndLoads)
{
    Base* saved = dataGeneration.Generator<Derived>().RandomHeap();
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
    BOOST_REQUIRE(loadedCasted->BaseValue() == savedCasted->BaseValue());
    BOOST_REQUIRE(loadedCasted->DerivedValue() == savedCasted->DerivedValue());

    delete saved;
    delete loaded;
}

BOOST_AUTO_TEST_SUITE_END()

namespace Inscription
{
    void Scribe<::BinaryPolymorphicManualRegistrationFixture::Base, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object.baseValue);
    }

    void Scribe<::BinaryPolymorphicManualRegistrationFixture::Derived, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::BinaryPolymorphicManualRegistrationFixture::Base>(object, archive);
        archive(object.derivedValue);
    }

    const Scribe<::BinaryPolymorphicManualRegistrationFixture::Derived, BinaryArchive>::ClassNameResolver
        Scribe<::BinaryPolymorphicManualRegistrationFixture::Derived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("CustomConstructionDerived");
}