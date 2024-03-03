#include <catch.hpp>

#include <Inscription/CompositeScribeCategory.h>
#include <Inscription/PointerScribe.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/StringScribe.h>

#include "GeneralFixture.h"

class BinaryPolymorphicManualRegistrationFixture : public GeneralFixture
{
public:
    Inscription::TypeRegistrationContext<Inscription::Format::Binary> typeRegistrationContext;

    BinaryPolymorphicManualRegistrationFixture()
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

        [[nodiscard]] int BaseValue() const
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

        [[nodiscard]] const std::string& DerivedValue() const
        {
            return derivedValue;
        }
    private:
        std::string derivedValue;
    private:
        INSCRIPTION_ACCESS;
    };
};

BinaryPolymorphicManualRegistrationFixture::Base::~Base() = default;

namespace Inscription
{
    template<>
    class Scribe<BinaryPolymorphicManualRegistrationFixture::Base>
    {
    public:
        using ObjectT = BinaryPolymorphicManualRegistrationFixture::Base;
    public:
        void Scriven(ObjectT& object, Format::Binary& format);
    };

    template<class Format>
    struct ScribeTraits<BinaryPolymorphicManualRegistrationFixture::Base, Format> final
    {
        using Category = CompositeScribeCategory<BinaryPolymorphicManualRegistrationFixture::Base>;
    };

    template<>
    class Scribe<BinaryPolymorphicManualRegistrationFixture::Derived> final
    {
    public:
        using ObjectT = BinaryPolymorphicManualRegistrationFixture::Derived;
    public:
        static Type OutputType(const Format::Binary& format);
    public:
        void Scriven(ObjectT& object, Format::Binary& format);
    };

    template<class Format>
    struct ScribeTraits<BinaryPolymorphicManualRegistrationFixture::Derived, Format> final
    {
        using Category = CompositeScribeCategory<BinaryPolymorphicManualRegistrationFixture::Derived>;
    };
}

SCENARIO_METHOD(
    BinaryPolymorphicManualRegistrationFixture,
    "loading manually registered type in binary",
    "[binary][pointer][polymorphic]"
) {
    GIVEN("saved manually registered polymorphic object")
    {
        Base* saved = dataGeneration.Generator<Derived>().RandomHeap();
        Derived* savedCasted = dynamic_cast<Derived*>(saved);

        Inscription::Binary::ToFile(saved, "Test.dat", typeRegistrationContext);

        WHEN("loading manually registered polymorphic object")
        {
            Base* loaded = nullptr;

            Inscription::Binary::FromFile(loaded, "Test.dat", typeRegistrationContext);

            Derived* loadedCasted = dynamic_cast<Derived*>(loaded);

            THEN("loaded object is valid")
            {
                REQUIRE(loadedCasted != nullptr);
                REQUIRE(loadedCasted->BaseValue() == savedCasted->BaseValue());
                REQUIRE(loadedCasted->DerivedValue() == savedCasted->DerivedValue());

                delete saved;
                delete loaded;
            }
        }
    }
}

namespace Inscription
{
    void Scribe<BinaryPolymorphicManualRegistrationFixture::Base>::Scriven(ObjectT& object, Format::Binary& format)
    {
        format(object.baseValue);
    }

    Type Scribe<BinaryPolymorphicManualRegistrationFixture::Derived>::OutputType(const Format::Binary& format)
    {
        return "CustomConstructionDerived";
    }

    void Scribe<BinaryPolymorphicManualRegistrationFixture::Derived>::Scriven(ObjectT& object, Format::Binary& format)
    {
        BaseScriven<::BinaryPolymorphicManualRegistrationFixture::Base>(object, format);
        format(object.derivedValue);
    }
}