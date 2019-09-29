#include <catch.hpp>
#include <utility>

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
        Derived(int baseValue = 0, std::string derivedValue = "") :
            Base(baseValue), derivedValue(std::move(derivedValue))
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

    template<size_t differentiator>
    class GeneralDerived : public Base
    {
    public:
        static ::Inscription::TypeHandle principleTypeHandle;
        static std::vector<::Inscription::TypeHandle> representedTypeHandles;

        static void Reset()
        {
            principleTypeHandle = "GeneralDerived";
            representedTypeHandles.clear();
        }
    public:
        GeneralDerived(int baseValue = 0, std::string derivedValue = "") :
            Base(baseValue), derivedValue(std::move(derivedValue))
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
    class UnregisteredBase
    {
    public:
        virtual ~UnregisteredBase() = 0;
    };

    class UnregisteredDerived : public UnregisteredBase
    {};
};

BinaryPolymorphicFixture::Base::~Base() = default;

template<size_t differentiator>
::Inscription::TypeHandle BinaryPolymorphicFixture::GeneralDerived<differentiator>::
    principleTypeHandle = "GeneralDerived";

template<size_t differentiator>
std::vector<::Inscription::TypeHandle> BinaryPolymorphicFixture::GeneralDerived<differentiator>::
    representedTypeHandles;

BinaryPolymorphicFixture::UnregisteredBase::~UnregisteredBase() = default;

namespace Inscription
{
    template<>
    class Scribe<::BinaryPolymorphicFixture::Base, BinaryArchive> final : public
        CompositeScribe<::BinaryPolymorphicFixture::Base, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };

    template<>
    class Scribe<::BinaryPolymorphicFixture::Derived, BinaryArchive> final : public
        CompositeScribe<::BinaryPolymorphicFixture::Derived, BinaryArchive>
    {
    public:
        static TypeHandle PrincipleTypeHandle(const ArchiveT& archive);
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };

    template<size_t differentiator>
    class Scribe<::BinaryPolymorphicFixture::GeneralDerived<differentiator>, BinaryArchive> final : public
        CompositeScribe<::BinaryPolymorphicFixture::GeneralDerived<differentiator>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<::BinaryPolymorphicFixture::GeneralDerived<differentiator>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;

        static TypeHandle PrincipleTypeHandle(const ArchiveT& archive);
        static std::vector<TypeHandle> RepresentedTypeHandles(const ArchiveT& archive);
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };

    template<>
    class Scribe<::BinaryPolymorphicFixture::UnregisteredBase, BinaryArchive> final : public
        CompositeScribe<::BinaryPolymorphicFixture::UnregisteredBase, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {}
    };

    template<>
    class Scribe<::BinaryPolymorphicFixture::UnregisteredDerived, BinaryArchive> final : public
        CompositeScribe<::BinaryPolymorphicFixture::UnregisteredDerived, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {}
    };
}

SCENARIO_METHOD(BinaryPolymorphicFixture, "loading polymorphic pointers in binary", "[binary][pointer][polymorphic]")
{
    GIVEN("saved null polymorphic pointer as base")
    {
        Base* saved = nullptr;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading pointer")
        {
            Base* loaded = nullptr;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loaded);
            }

            THEN("loaded pointer is null")
            {
                REQUIRE(loaded == nullptr);
            }
        }
    }

    GIVEN("saved null polymorphic pointer as derived")
    {
        Derived* saved = nullptr;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading pointer")
        {
            Derived* loaded = nullptr;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loaded);
            }

            THEN("loaded pointer is null")
            {
                REQUIRE(loaded == nullptr);
            }
        }
    }

    GIVEN("saved valid polymorphic pointer")
    {
        Base* saved = dataGeneration.RandomHeap<Derived>();
        Derived* savedCasted = dynamic_cast<Derived*>(saved);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        THEN("loading pointer")
        {
            Base* loaded = nullptr;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loaded);
            }

            THEN("loaded pointer is valid")
            {
                Derived* loadedCasted = dynamic_cast<Derived*>(loaded);
                REQUIRE(loadedCasted != nullptr);
                REQUIRE(loadedCasted->BaseValue() == savedCasted->BaseValue());
                REQUIRE(loadedCasted->DerivedValue() == savedCasted->DerivedValue());

                delete saved;
                delete loaded;
            }
        }
    }

    GIVEN("output archive")
    {
        auto outputArchive = CreateRegistered<OutputArchive>();

        WHEN("saving unregistered type through pointer")
        {
            THEN("throws error")
            {
                UnregisteredBase* saved = dataGeneration.Generator<UnregisteredDerived>().RandomHeap();
                REQUIRE_THROWS_AS(outputArchive(saved), ::Inscription::RepresentedTypeHandleNotFound);
                delete saved;
            }
        }
    }
}

SCENARIO_METHOD(
    BinaryPolymorphicFixture,
    "loading polymorphic objects with specific type handles",
    "[binary][pointer][polymorphic]"
) {
    GIVEN("multiple saved polymorphic pointers")
    {
        using Saved0 = GeneralDerived<0>;
        using Saved1 = GeneralDerived<1>;
        using Saved2 = GeneralDerived<2>;

        Base* saved0 = dataGeneration.RandomHeap<Saved0, int, std::string>();
        Base* saved1 = dataGeneration.RandomHeap<Saved1, int, std::string>();
        Base* saved2 = dataGeneration.RandomHeap<Saved2, int, std::string>();
        auto castedSaved0 = dynamic_cast<Saved0*>(saved0);
        auto castedSaved1 = dynamic_cast<Saved1*>(saved1);
        auto castedSaved2 = dynamic_cast<Saved2*>(saved2);

        auto saved0PrincipleTypeHandle = "Saved0PrincipleTypeHandle";
        auto saved1PrincipleTypeHandle = "Saved1PrincipleTypeHandle";
        auto saved2PrincipleTypeHandle = "Saved2PrincipleTypeHandle";

        Saved0::principleTypeHandle = saved0PrincipleTypeHandle;
        Saved1::principleTypeHandle = saved1PrincipleTypeHandle;
        Saved2::principleTypeHandle = saved2PrincipleTypeHandle;

        {
            auto outputArchive = ::Inscription::OutputBinaryArchive("Test.dat", "testing", 1);
            outputArchive.RegisterType<Base>();
            outputArchive.RegisterType<Saved0>();
            outputArchive.RegisterType<Saved1>();
            outputArchive.RegisterType<Saved2>();
            outputArchive(saved0);
            outputArchive(saved1);
            outputArchive(saved2);
        }

        GeneralDerived<0>::Reset();
        GeneralDerived<1>::Reset();
        GeneralDerived<2>::Reset();

        WHEN("loading unregistered pointer")
        {
            auto inputArchive = ::Inscription::InputBinaryArchive("Test.dat", "testing");

            Base* loaded = nullptr;

            THEN("throws error")
            {
                REQUIRE_THROWS_AS(inputArchive(loaded), ::Inscription::RepresentedTypeHandleNotFound);
            }
        }

        WHEN("loading pointer through different class with same principle type handle")
        {
            using SamePrincipleTypeHandle = GeneralDerived<0>;
            SamePrincipleTypeHandle::principleTypeHandle = saved0PrincipleTypeHandle;

            Base* loaded = nullptr;

            {
                auto inputArchive = ::Inscription::InputBinaryArchive("Test.dat", "testing");
                inputArchive.RegisterType<Base>();
                inputArchive.RegisterType<SamePrincipleTypeHandle>();
                inputArchive(loaded);
            }

            THEN("loaded pointer is same as saved")
            {
                auto castedLoaded = dynamic_cast<SamePrincipleTypeHandle*>(loaded);
                REQUIRE(castedLoaded != nullptr);
                REQUIRE(loaded->BaseValue() == saved0->BaseValue());
                REQUIRE(castedLoaded->DerivedValue() == castedSaved0->DerivedValue());
            }
        }

        WHEN("loading pointer through different class with saved principle type handle in represented type handles")
        {
            using InRepresentedTypeHandles = GeneralDerived<0>;
            InRepresentedTypeHandles::representedTypeHandles.emplace_back(saved0PrincipleTypeHandle);

            Base* loaded = nullptr;

            {
                auto inputArchive = ::Inscription::InputBinaryArchive("Test.dat", "testing");
                inputArchive.RegisterType<Base>();
                inputArchive.RegisterType<InRepresentedTypeHandles>();
                inputArchive(loaded);
            }

            THEN("loaded pointer is same as saved")
            {
                auto castedLoaded = dynamic_cast<InRepresentedTypeHandles*>(loaded);
                REQUIRE(castedLoaded != nullptr);
                REQUIRE(loaded->BaseValue() == saved0->BaseValue());
                REQUIRE(castedLoaded->DerivedValue() == castedSaved0->DerivedValue());
            }
        }

        WHEN("loading multiple different principle type handle classes through single represented type")
        {
            using InRepresentedTypeHandles = GeneralDerived<0>;
            InRepresentedTypeHandles::representedTypeHandles.emplace_back(saved0PrincipleTypeHandle);
            InRepresentedTypeHandles::representedTypeHandles.emplace_back(saved1PrincipleTypeHandle);
            InRepresentedTypeHandles::representedTypeHandles.emplace_back(saved2PrincipleTypeHandle);

            Base* loaded0 = nullptr;
            Base* loaded1 = nullptr;
            Base* loaded2 = nullptr;

            {
                auto inputArchive = ::Inscription::InputBinaryArchive("Test.dat", "testing");
                inputArchive.RegisterType<Base>();
                inputArchive.RegisterType<InRepresentedTypeHandles>();
                inputArchive(loaded0);
                inputArchive(loaded1);
                inputArchive(loaded2);
            }

            THEN("loaded pointer is same as saved")
            {
                auto castedLoaded0 = dynamic_cast<InRepresentedTypeHandles*>(loaded0);
                auto castedLoaded1 = dynamic_cast<InRepresentedTypeHandles*>(loaded1);
                auto castedLoaded2 = dynamic_cast<InRepresentedTypeHandles*>(loaded2);
                REQUIRE(castedLoaded0 != nullptr);
                REQUIRE(castedLoaded1 != nullptr);
                REQUIRE(castedLoaded2 != nullptr);
                REQUIRE(loaded0->BaseValue() == saved0->BaseValue());
                REQUIRE(loaded1->BaseValue() == saved1->BaseValue());
                REQUIRE(loaded2->BaseValue() == saved2->BaseValue());
                REQUIRE(castedLoaded0->DerivedValue() == castedSaved0->DerivedValue());
                REQUIRE(castedLoaded1->DerivedValue() == castedSaved1->DerivedValue());
                REQUIRE(castedLoaded2->DerivedValue() == castedSaved2->DerivedValue());
            }
        }
    }

    GIVEN("output archive")
    {
        auto outputArchive = ::Inscription::OutputBinaryArchive("Test.dat", "testing", 1);

        WHEN("registering same type handle in principle and represented handles on same type")
        {
            ::Inscription::TypeHandle typeHandle(dataGeneration.Random<std::string>());

            using Type = GeneralDerived<0>;
            Type::Reset();
            Type::principleTypeHandle = typeHandle;
            Type::representedTypeHandles.push_back(typeHandle);

            THEN("does not throw")
            {
                REQUIRE_NOTHROW(outputArchive.RegisterType<Type>());
            }
        }

        WHEN("registering same type handle in represented handles on same type throws")
        {
            ::Inscription::TypeHandle typeHandle(dataGeneration.Random<std::string>());

            using Type = GeneralDerived<0>;
            Type::Reset();
            Type::representedTypeHandles.push_back(typeHandle);
            Type::representedTypeHandles.push_back(typeHandle);

            THEN("throws error")
            {
                REQUIRE_THROWS_AS(
                    outputArchive.RegisterType<Type>(),
                    ::Inscription::RepresentedTypeHandlesAlreadyRegistered);
            }
        }

        WHEN("registering same type handle as principle type handle in two separate types")
        {
            ::Inscription::TypeHandle typeHandle(dataGeneration.Random<std::string>());

            using Type0 = GeneralDerived<0>;
            Type0::Reset();
            Type0::principleTypeHandle = typeHandle;

            using Type1 = GeneralDerived<1>;
            Type1::Reset();
            Type1::principleTypeHandle = typeHandle;

            THEN("throws error on second register")
            {
                REQUIRE_NOTHROW(outputArchive.RegisterType<Type0>());
                REQUIRE_THROWS_AS(
                    outputArchive.RegisterType<Type1>(),
                    ::Inscription::RepresentedTypeHandlesAlreadyRegistered);
            }
        }

        WHEN("registering same type handle as represented type handles in two separate types")
        {
            ::Inscription::TypeHandle typeHandle(dataGeneration.Random<std::string>());

            using Type0 = GeneralDerived<0>;
            Type0::Reset();
            Type0::representedTypeHandles.push_back(typeHandle);

            using Type1 = GeneralDerived<1>;
            Type1::Reset();
            Type1::representedTypeHandles.push_back(typeHandle);

            THEN("throws error on second register")
            {
                REQUIRE_NOTHROW(outputArchive.RegisterType<Type0>());
                REQUIRE_THROWS_AS(
                    outputArchive.RegisterType<Type1>(),
                    ::Inscription::RepresentedTypeHandlesAlreadyRegistered);
            }
        }

        WHEN("registering same type handle as principle type handle and represented type handle in two separate types")
        {
            ::Inscription::TypeHandle typeHandle(dataGeneration.Random<std::string>());

            using Type0 = GeneralDerived<0>;
            Type0::Reset();
            Type0::principleTypeHandle = typeHandle;

            using Type1 = GeneralDerived<1>;
            Type1::Reset();
            Type1::representedTypeHandles.push_back(typeHandle);

            THEN("throws error on second register")
            {
                REQUIRE_NOTHROW(outputArchive.RegisterType<Type0>());
                REQUIRE_THROWS_AS(
                    outputArchive.RegisterType<Type1>(),
                    ::Inscription::RepresentedTypeHandlesAlreadyRegistered);
            }
        }
    }
}

namespace Inscription
{
    void Scribe<::BinaryPolymorphicFixture::Base, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object.baseValue);
    }

    TypeHandle Scribe<::BinaryPolymorphicFixture::Derived, BinaryArchive>::PrincipleTypeHandle(
        const ArchiveT& archive
    ) {
        return "BinaryPolymorphicDerived";
    }

    void Scribe<::BinaryPolymorphicFixture::Derived, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::BinaryPolymorphicFixture::Base>(object, archive);
        archive(object.derivedValue);
    }

    template<size_t differentiator>
    TypeHandle Scribe<::BinaryPolymorphicFixture::GeneralDerived<differentiator>, BinaryArchive>::PrincipleTypeHandle(
        const ArchiveT& archive)
    {
        return ObjectT::principleTypeHandle;
    }

    template<size_t differentiator>
    auto Scribe<::BinaryPolymorphicFixture::GeneralDerived<differentiator>, BinaryArchive>::RepresentedTypeHandles(
        const ArchiveT& archive)
        -> std::vector<TypeHandle>
    {
        return ObjectT::representedTypeHandles;
    }

    template<size_t differentiator>
    void Scribe<::BinaryPolymorphicFixture::GeneralDerived<differentiator>, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::BinaryPolymorphicFixture::Base>(object, archive);
        archive(object.derivedValue);
    }
}