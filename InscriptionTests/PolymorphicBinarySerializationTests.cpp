#include <boost/test/unit_test.hpp>

#include <Inscription/Memory.h>
#include <Inscription/Numeric.h>
#include <Inscription/Pointer.h>
#include <Inscription/String.h>

#include "BinaryFixture.h"

class PolymorphicBinarySerializationTestsFixture : public BinaryFixture
{
public:
    PolymorphicBinarySerializationTestsFixture()
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

    class DerivedComposite : public Base
    {
    public:
        DerivedComposite(int baseValue, std::unique_ptr<Base>&& ptr) :
            Base(baseValue), ptr(std::move(ptr))
        {}

        const std::unique_ptr<Base>& Ptr() const
        {
            return ptr;
        }
    private:
        std::unique_ptr<Base> ptr;
    private:
        INSCRIPTION_ACCESS;
    };
};

PolymorphicBinarySerializationTestsFixture::Base::~Base()
{}

namespace Inscription
{
    template<>
    class Scribe<::PolymorphicBinarySerializationTestsFixture::Base, BinaryArchive> : public
        CompositeScribe<::PolymorphicBinarySerializationTestsFixture::Base, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };

    template<>
    class Scribe<::PolymorphicBinarySerializationTestsFixture::Derived, BinaryArchive> : public
        CompositeScribe<::PolymorphicBinarySerializationTestsFixture::Derived, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
        static const ClassNameResolver classNameResolver;
    };
}

BOOST_FIXTURE_TEST_SUITE(PolymorphicBinarySerializationTests, PolymorphicBinarySerializationTestsFixture)

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
    void Scribe<::PolymorphicBinarySerializationTestsFixture::Base, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        archive(object.baseValue);
    }

    void Scribe<::PolymorphicBinarySerializationTestsFixture::Derived, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::PolymorphicBinarySerializationTestsFixture::Base>(object, archive);
        archive(object.derivedValue);
    }

    const Scribe<::PolymorphicBinarySerializationTestsFixture::Derived, BinaryArchive>::ClassNameResolver
        Scribe<::PolymorphicBinarySerializationTestsFixture::Derived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("Derived");
}