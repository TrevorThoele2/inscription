#include <boost/test/unit_test.hpp>

#include <Inscription/Numeric.h>
#include <Inscription/Pointer.h>
#include <Inscription/String.h>

#include "BinaryFixture.h"

class ManualRegistrationPolymorphicBinarySerializationTestsFixture : public BinaryFixture
{
public:
    ManualRegistrationPolymorphicBinarySerializationTestsFixture()
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

ManualRegistrationPolymorphicBinarySerializationTestsFixture::Base::~Base()
{}

namespace Inscription
{
    template<>
    class Scribe<::ManualRegistrationPolymorphicBinarySerializationTestsFixture::Base, BinaryArchive> : public
        CompositeScribe<::ManualRegistrationPolymorphicBinarySerializationTestsFixture::Base, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };

    template<>
    class Scribe<::ManualRegistrationPolymorphicBinarySerializationTestsFixture::Derived, BinaryArchive> : public
        CompositeScribe<::ManualRegistrationPolymorphicBinarySerializationTestsFixture::Derived, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
        static const ClassNameResolver classNameResolver;
    };
}

BOOST_FIXTURE_TEST_SUITE(ManualRegistrationPolymorphicBinarySerializationTests, ManualRegistrationPolymorphicBinarySerializationTestsFixture)

BOOST_AUTO_TEST_CASE(PolymorphicOwningPointer_SavesAndLoads)
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
    void Scribe<::ManualRegistrationPolymorphicBinarySerializationTestsFixture::Base, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        archive(object.baseValue);
    }

    void Scribe<::ManualRegistrationPolymorphicBinarySerializationTestsFixture::Derived, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::ManualRegistrationPolymorphicBinarySerializationTestsFixture::Base>(object, archive);
        archive(object.derivedValue);
    }

    const Scribe<::ManualRegistrationPolymorphicBinarySerializationTestsFixture::Derived, BinaryArchive>::ClassNameResolver
        Scribe<::ManualRegistrationPolymorphicBinarySerializationTestsFixture::Derived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("Derived");
}