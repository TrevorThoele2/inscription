#include <catch.hpp>

#include <Inscription/CompositeScribeCategory.h>
#include <Inscription/MemoryScribe.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/StringScribe.h>

#include "GeneralFixture.h"

class MemoryTestsFixture : public GeneralFixture
{
public:
    TestFramework::DataGeneration dataGeneration;
    Inscription::TypeRegistrationContext<Inscription::Format::Binary> typeRegistrationContext;

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

        Inscription::Binary::ToFile(saved, "Test.dat", typeRegistrationContext);

        WHEN("loading")
        {
            std::unique_ptr<Base> loaded;

            Inscription::Binary::FromFile(loaded, "Test.dat", typeRegistrationContext);

            const auto savedDerived = dynamic_cast<Derived*>(saved.get());
            const auto loadedDerived = dynamic_cast<Derived*>(loaded.get());

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
        void Scriven(ObjectT& object, Format::Binary& format)
        {
            format(object.baseValue);
        }
    };

    template<class Format>
    struct ScribeTraits<MemoryTestsFixture::Base, Format> final
    {
        using Category = CompositeScribeCategory<MemoryTestsFixture::Base>;
    };

    template<>
    class Scribe<MemoryTestsFixture::Derived> final
    {
    public:
        using ObjectT = MemoryTestsFixture::Derived;
    public:
        static Type OutputType(const Format::Binary& format);
    public:
        void Scriven(ObjectT& object, Format::Binary& format)
        {
            BaseScriven<MemoryTestsFixture::Base>(object, format);
            format(object.derivedValue);
        }
    };

    Type Scribe<MemoryTestsFixture::Derived>::OutputType(const Format::Binary& format)
    {
        return "MemoryDerived";
    }
}