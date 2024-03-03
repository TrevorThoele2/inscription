#include <catch.hpp>

#include <Inscription/CompositeScribe.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"
#include "Inscription/BaseScriven.h"

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
    [[nodiscard]] OutputArchive CreateRegistered() const
    {
        OutputArchive created("Test.dat");

        created.types.RegisterType<Base>();
        created.types.RegisterType<Derived>();

        return created;
    }

    template<>
    [[nodiscard]] InputArchive CreateRegistered() const
    {
        InputArchive created("Test.dat");

        created.types.RegisterType<Base>();
        created.types.RegisterType<Derived>();

        return created;
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
    class Scribe<::BinaryPolymorphicManualRegistrationFixture::Base, BinaryArchive> : public
        CompositeScribe<::BinaryPolymorphicManualRegistrationFixture::Base, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };

    template<>
    class Scribe<::BinaryPolymorphicManualRegistrationFixture::Derived, BinaryArchive> : public
        CompositeScribe<::BinaryPolymorphicManualRegistrationFixture::Derived, BinaryArchive>
    {
    public:
        static Type OutputType(const ArchiveT& archive);
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
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

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading manually registered polymorphic object")
        {
            Base* loaded = nullptr;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loaded);
            }

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
    void Scribe<::BinaryPolymorphicManualRegistrationFixture::Base, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object.baseValue);
    }

    Type Scribe<::BinaryPolymorphicManualRegistrationFixture::Derived, BinaryArchive>::OutputType(
        const ArchiveT& archive
    ) {
        return "CustomConstructionDerived";
    }

    void Scribe<::BinaryPolymorphicManualRegistrationFixture::Derived, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::BinaryPolymorphicManualRegistrationFixture::Base>(object, archive);
        archive(object.derivedValue);
    }
}