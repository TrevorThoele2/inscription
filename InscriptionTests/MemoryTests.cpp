#include <boost/test/unit_test.hpp>

#include <Inscription/MemoryScribe.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/StringScribe.h>
#include <Inscription/PointerScribe.h>

#include "BinaryFixture.h"

class MemoryTestsFixture : public BinaryFixture
{
public:
    ::TestFramework::DataGeneration dataGeneration;

    MemoryTestsFixture()
    {
        typeRegistrationContext.RegisterType<Base>();
        typeRegistrationContext.RegisterType<Derived>();
    }

    class Base
    {
    public:
        int baseValue = 0;
    public:
        virtual ~Base() = 0;
    };

    class Derived : public Base
    {
    public:
        std::string derivedValue;
    };
};

MemoryTestsFixture::Base::~Base()
{}

BOOST_FIXTURE_TEST_SUITE(MemoryTests, MemoryTestsFixture)

BOOST_AUTO_TEST_CASE(UniquePtrSavesAndLoads_ThroughBaseClassPointer)
{
    std::unique_ptr<Base> saved(new Derived());

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(saved);
    }

    {
        std::unique_ptr<Base> loaded;

        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loaded);

        Derived* savedDerived = static_cast<Derived*>(saved.get());
        Derived* loadedDerived = static_cast<Derived*>(loaded.get());

        BOOST_REQUIRE(loaded.get() != nullptr);
        BOOST_REQUIRE(loaded->baseValue == saved->baseValue);
        BOOST_REQUIRE(loadedDerived->derivedValue == savedDerived->derivedValue);
    }
}

BOOST_AUTO_TEST_SUITE_END()

namespace Inscription
{
    template<>
    class Scribe<::MemoryTestsFixture::Base, BinaryArchive> :
        public CompositeScribe<::MemoryTestsFixture::Base, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive(object.baseValue);
        }

        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
        {
            DoBasicConstruction(storage, archive);
        }
    };

    template<>
    class Scribe<::MemoryTestsFixture::Derived, BinaryArchive> :
        public CompositeScribe<::MemoryTestsFixture::Derived, BinaryArchive>
    {
    public:
        static ClassNameResolver classNameResolver;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            BaseScriven<::MemoryTestsFixture::Base>(object, archive);
            archive(object.derivedValue);
        }

        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
        {
            DoBasicConstruction(storage, archive);
        }
    };

    Scribe<::MemoryTestsFixture::Derived, BinaryArchive>::ClassNameResolver
        Scribe<::MemoryTestsFixture::Derived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("CustomConstructionDerived");
}