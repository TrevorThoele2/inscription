#include <catch.hpp>

#include <Inscription/MemoryScribe.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"
#include "Inscription/BaseScriven.h"

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

SCENARIO_METHOD(MemoryTestsFixture, "unique_ptr loads after saving", "[binary][std][unique_ptr]")
{
    GIVEN("saved unique_ptr")
    {
        std::unique_ptr<Base> saved(new Derived());

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
        {
            std::unique_ptr<Base> loaded;

            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);

            auto savedDerived = dynamic_cast<Derived*>(saved.get());
            auto loadedDerived = dynamic_cast<Derived*>(loaded.get());

            THEN("is valid")
            {
                REQUIRE(loaded.get() != nullptr);
                REQUIRE(loaded->baseValue == saved->baseValue);
                REQUIRE(loadedDerived->derivedValue == savedDerived->derivedValue);
            }
        }
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
    };

    template<>
    class Scribe<::MemoryTestsFixture::Derived, BinaryArchive> :
        public CompositeScribe<::MemoryTestsFixture::Derived, BinaryArchive>
    {
    public:
        static Type OutputType(const ArchiveT& archive);
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            BaseScriven<::MemoryTestsFixture::Base>(object, archive);
            archive(object.derivedValue);
        }
    };

    Type Scribe<::MemoryTestsFixture::Derived, BinaryArchive>::OutputType(
        const ArchiveT& archive
    ) {
        return "MemoryDerived";
    }
}