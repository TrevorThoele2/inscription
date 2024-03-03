#include <catch.hpp>

#include <Inscription/MemoryScribe.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/PointerScribe.h>
#include <Inscription/StringScribe.h>
#include <Inscription/BaseScriven.h>

#include "BinaryFixture.h"

class BinaryPolymorphicFixture : public BinaryFixture
{
public:
    BinaryPolymorphicFixture()
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

    class UnregisteredBase
    {
    public:
        virtual ~UnregisteredBase() = 0;
    };

    class UnregisteredDerived : public UnregisteredBase
    {};
};

BinaryPolymorphicFixture::Base::~Base()
{}

BinaryPolymorphicFixture::UnregisteredBase::~UnregisteredBase()
{}

namespace Inscription
{
    template<>
    class Scribe<::BinaryPolymorphicFixture::Base, BinaryArchive> : public
        CompositeScribe<::BinaryPolymorphicFixture::Base, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive);
    };

    template<>
    class Scribe<::BinaryPolymorphicFixture::Derived, BinaryArchive> : public
        CompositeScribe<::BinaryPolymorphicFixture::Derived, BinaryArchive>
    {
    public:
        static const ClassNameResolver classNameResolver;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };

    template<>
    class Scribe<::BinaryPolymorphicFixture::UnregisteredBase, BinaryArchive> : public
        CompositeScribe<::BinaryPolymorphicFixture::UnregisteredBase, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {}
    };

    template<>
    class Scribe<::BinaryPolymorphicFixture::UnregisteredDerived, BinaryArchive> : public
        CompositeScribe<::BinaryPolymorphicFixture::UnregisteredDerived, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {}
    };
}

TEST_CASE_METHOD(BinaryPolymorphicFixture, "binary polymorphic")
{
    SECTION("polymorphic base null pointer saves and loads")
    {
        Base* saved = nullptr;

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

        REQUIRE(loadedCasted == nullptr);
    }

    SECTION("polymorphic derived null pointer saves and loads")
    {
        Derived* saved = nullptr;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        Derived* loaded = nullptr;

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);
        }

        REQUIRE(loaded == nullptr);
    }

    SECTION("polymorphic pointer saves and loads")
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

        REQUIRE(loadedCasted != nullptr);
        REQUIRE(loadedCasted->BaseValue() == savedCasted->BaseValue());
        REQUIRE(loadedCasted->DerivedValue() == savedCasted->DerivedValue());

        delete saved;
        delete loaded;
    }

    SECTION("unregistered type throws exception on save")
    {
        UnregisteredBase* saved = dataGeneration.Generator<UnregisteredDerived>().RandomHeap();

        auto outputArchive = CreateRegistered<OutputArchive>();

        REQUIRE_THROWS_AS(outputArchive(saved), ::Inscription::PolymorphicTypeNotFound);

        delete saved;
    }
}

namespace Inscription
{
    void Scribe<::BinaryPolymorphicFixture::Base, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object.baseValue);
    }

    const Scribe<::BinaryPolymorphicFixture::Derived, BinaryArchive>::ClassNameResolver
        Scribe<::BinaryPolymorphicFixture::Derived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("CustomConstructionDerived");

    void Scribe<::BinaryPolymorphicFixture::Derived, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::BinaryPolymorphicFixture::Base>(object, archive);
        archive(object.derivedValue);
    }
}