#include <catch.hpp>

#include <Inscription/MemoryScribe.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/StringScribe.h>
#include <Inscription/PointerScribe.h>
#include <Inscription/BaseScriven.h>

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

    class Derived final : public Base
    {
    public:
        std::string derivedValue;
    };
};

MemoryTestsFixture::Base::~Base() = default;

TEST_CASE_METHOD(MemoryTestsFixture, "unique_ptr saves and loads through base class pointer")
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

        auto savedDerived = static_cast<Derived*>(saved.get());
        auto loadedDerived = static_cast<Derived*>(loaded.get());

        REQUIRE(loaded.get() != nullptr);
        REQUIRE(loaded->baseValue == saved->baseValue);
        REQUIRE(loadedDerived->derivedValue == savedDerived->derivedValue);
    }
}

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