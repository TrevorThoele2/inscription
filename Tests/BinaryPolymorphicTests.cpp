#include <catch.hpp>
#include <utility>

#include <Inscription/MemoryScribe.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/PointerScribe.h>
#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"
#include "Inscription/BaseScriven.h"

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
        static ::Inscription::Type outputType;
        static std::vector<::Inscription::Type> inputTypes;

        static void Reset()
        {
            outputType = "GeneralDerived";
            inputTypes.clear();
        }
    public:
        explicit GeneralDerived(int baseValue = 0, std::string derivedValue = "") :
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
::Inscription::Type BinaryPolymorphicFixture::GeneralDerived<differentiator>::
    outputType = "GeneralDerived";

template<size_t differentiator>
std::vector<::Inscription::Type> BinaryPolymorphicFixture::GeneralDerived<differentiator>::
    inputTypes;

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
        static Type OutputType(const ArchiveT& archive);
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

        static Type OutputType(const ArchiveT& archive);
        static std::vector<Type> InputTypes(const ArchiveT& archive);
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

SCENARIO_METHOD
(
    BinaryPolymorphicFixture,
    "loading polymorphic pointers in binary",
    "[binary][pointer][polymorphic]"
) {
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

        WHEN("loading pointer")
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
                REQUIRE_THROWS_AS(outputArchive(saved), ::Inscription::InputTypeNotFound);
                delete saved;
            }
        }
    }
}

SCENARIO_METHOD
(
    BinaryPolymorphicFixture,
    "loading polymorphic objects with specific type handles",
    "[binary][pointer][polymorphic]"
) {
    GIVEN("multiple saved polymorphic pointers")
    {
        using Saved0 = GeneralDerived<0>;
        using Saved1 = GeneralDerived<1>;
        using Saved2 = GeneralDerived<2>;

        auto saved0 = std::unique_ptr<Base>(dataGeneration.RandomHeap<Saved0, int, std::string>());
        auto saved1 = std::unique_ptr<Base>(dataGeneration.RandomHeap<Saved1, int, std::string>());
        auto saved2 = std::unique_ptr<Base>(dataGeneration.RandomHeap<Saved2, int, std::string>());
        auto castedSaved0 = dynamic_cast<Saved0*>(saved0.get());
        auto castedSaved1 = dynamic_cast<Saved1*>(saved1.get());
        auto castedSaved2 = dynamic_cast<Saved2*>(saved2.get());

        auto saved0OutputType = "Saved0OutputType";
        auto saved1OutputType = "Saved1OutputType";
        auto saved2OutputType = "Saved2OutputType";

        Saved0::outputType = saved0OutputType;
        Saved1::outputType = saved1OutputType;
        Saved2::outputType = saved2OutputType;

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
                REQUIRE_THROWS_AS(inputArchive(loaded), ::Inscription::InputTypeNotFound);
            }
        }

        WHEN("loading pointer through different class with same output type handle")
        {
            using SameOutputType = GeneralDerived<0>;
            SameOutputType::outputType = saved0OutputType;

            std::unique_ptr<Base> loaded = nullptr;

            {
                auto inputArchive = ::Inscription::InputBinaryArchive("Test.dat", "testing");
                inputArchive.RegisterType<Base>();
                inputArchive.RegisterType<SameOutputType>();
                inputArchive(loaded);
            }

            THEN("loaded pointer is same as saved")
            {
                auto castedLoaded = dynamic_cast<SameOutputType*>(loaded.get());
                REQUIRE(castedLoaded != nullptr);
                REQUIRE(loaded->BaseValue() == saved0->BaseValue());
                REQUIRE(castedLoaded->DerivedValue() == castedSaved0->DerivedValue());
            }
        }

        WHEN("loading pointer through different class with saved output type handle in input type handles")
        {
            using InInputTypes = GeneralDerived<0>;
            InInputTypes::inputTypes.emplace_back(saved0OutputType);

            std::unique_ptr<Base> loaded = nullptr;

            {
                auto inputArchive = ::Inscription::InputBinaryArchive("Test.dat", "testing");
                inputArchive.RegisterType<Base>();
                inputArchive.RegisterType<InInputTypes>();
                inputArchive(loaded);
            }

            THEN("loaded pointer is same as saved")
            {
                auto castedLoaded = dynamic_cast<InInputTypes*>(loaded.get());
                REQUIRE(castedLoaded != nullptr);
                REQUIRE(loaded->BaseValue() == saved0->BaseValue());
                REQUIRE(castedLoaded->DerivedValue() == castedSaved0->DerivedValue());
            }
        }

        WHEN("loading multiple different output type handle classes through single input type")
        {
            using InInputTypes = GeneralDerived<0>;
            InInputTypes::inputTypes.emplace_back(saved0OutputType);
            InInputTypes::inputTypes.emplace_back(saved1OutputType);
            InInputTypes::inputTypes.emplace_back(saved2OutputType);

            std::unique_ptr<Base> loaded0 = nullptr;
            std::unique_ptr<Base> loaded1 = nullptr;
            std::unique_ptr<Base> loaded2 = nullptr;

            {
                auto inputArchive = ::Inscription::InputBinaryArchive("Test.dat", "testing");
                inputArchive.RegisterType<Base>();
                inputArchive.RegisterType<InInputTypes>();
                inputArchive(loaded0);
                inputArchive(loaded1);
                inputArchive(loaded2);
            }

            THEN("loaded pointer is same as saved")
            {
                auto castedLoaded0 = dynamic_cast<InInputTypes*>(loaded0.get());
                auto castedLoaded1 = dynamic_cast<InInputTypes*>(loaded1.get());
                auto castedLoaded2 = dynamic_cast<InInputTypes*>(loaded2.get());
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

        WHEN("registering same type handle in output and input handles on same type")
        {
            ::Inscription::Type type(dataGeneration.Random<std::string>());

            using Type = GeneralDerived<0>;
            Type::Reset();
            Type::outputType = type;
            Type::inputTypes.push_back(type);

            THEN("does not throw")
            {
                REQUIRE_NOTHROW(outputArchive.RegisterType<Type>());
            }
        }

        WHEN("registering same type handle in input handles on same type throws")
        {
            ::Inscription::Type type(dataGeneration.Random<std::string>());

            using Type = GeneralDerived<0>;
            Type::Reset();
            Type::inputTypes.push_back(type);
            Type::inputTypes.push_back(type);

            THEN("throws error")
            {
                REQUIRE_THROWS_AS(
                    outputArchive.RegisterType<Type>(),
                    ::Inscription::InputTypesAlreadyRegistered);
            }
        }

        WHEN("registering same type handle as output type handle in two separate types")
        {
            ::Inscription::Type type(dataGeneration.Random<std::string>());

            using Type0 = GeneralDerived<0>;
            Type0::Reset();
            Type0::outputType = type;

            using Type1 = GeneralDerived<1>;
            Type1::Reset();
            Type1::outputType = type;

            THEN("throws error on second register")
            {
                REQUIRE_NOTHROW(outputArchive.RegisterType<Type0>());
                REQUIRE_THROWS_AS(
                    outputArchive.RegisterType<Type1>(),
                    ::Inscription::InputTypesAlreadyRegistered);
            }
        }

        WHEN("registering same type handle as input type handles in two separate types")
        {
            ::Inscription::Type type(dataGeneration.Random<std::string>());

            using Type0 = GeneralDerived<0>;
            Type0::Reset();
            Type0::inputTypes.push_back(type);

            using Type1 = GeneralDerived<1>;
            Type1::Reset();
            Type1::inputTypes.push_back(type);

            THEN("throws error on second register")
            {
                REQUIRE_NOTHROW(outputArchive.RegisterType<Type0>());
                REQUIRE_THROWS_AS(
                    outputArchive.RegisterType<Type1>(),
                    ::Inscription::InputTypesAlreadyRegistered);
            }
        }

        WHEN("registering same type handle as output type handle and input type handle in two separate types")
        {
            ::Inscription::Type type(dataGeneration.Random<std::string>());

            using Type0 = GeneralDerived<0>;
            Type0::Reset();
            Type0::outputType = type;

            using Type1 = GeneralDerived<1>;
            Type1::Reset();
            Type1::inputTypes.push_back(type);

            THEN("throws error on second register")
            {
                REQUIRE_NOTHROW(outputArchive.RegisterType<Type0>());
                REQUIRE_THROWS_AS(
                    outputArchive.RegisterType<Type1>(),
                    ::Inscription::InputTypesAlreadyRegistered);
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

    Type Scribe<::BinaryPolymorphicFixture::Derived, BinaryArchive>::OutputType(
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
    Type Scribe<::BinaryPolymorphicFixture::GeneralDerived<differentiator>, BinaryArchive>::OutputType(
        const ArchiveT& archive)
    {
        return ObjectT::outputType;
    }

    template<size_t differentiator>
    auto Scribe<::BinaryPolymorphicFixture::GeneralDerived<differentiator>, BinaryArchive>::InputTypes(
        const ArchiveT& archive)
        -> std::vector<Type>
    {
        return ObjectT::inputTypes;
    }

    template<size_t differentiator>
    void Scribe<::BinaryPolymorphicFixture::GeneralDerived<differentiator>, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::BinaryPolymorphicFixture::Base>(object, archive);
        archive(object.derivedValue);
    }
}