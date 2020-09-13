#include <catch.hpp>

#include <Inscription/CompositeScribeCategory.h>
#include <Inscription/MemoryScribe.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/StringScribe.h>

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
    class Scribe<MemoryTestsFixture::Base> final
    {
    public:
        using ObjectT = MemoryTestsFixture::Base;
    public:
        void Scriven(ObjectT& object, BinaryArchive& archive)
        {
            archive(object.baseValue);
        }
    };

    template<class Archive>
    struct ScribeTraits<MemoryTestsFixture::Base, Archive> final
    {
        using Category = CompositeScribeCategory<MemoryTestsFixture::Base>;
    };

    template<>
    class Scribe<MemoryTestsFixture::Derived> final
    {
    public:
        using ObjectT = MemoryTestsFixture::Derived;
    public:
        static Type OutputType(const BinaryArchive& archive);
    public:
        void Scriven(ObjectT& object, BinaryArchive& archive)
        {
            BaseScriven<::MemoryTestsFixture::Base>(object, archive);
            archive(object.derivedValue);
        }
    };

    Type Scribe<MemoryTestsFixture::Derived>::OutputType(const BinaryArchive& archive)
    {
        return "MemoryDerived";
    }
}