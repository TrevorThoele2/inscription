#include <catch.hpp>

#include <Inscription/NumericScribe.h>
#include <Inscription/PointerScribe.h>
#include <Inscription/StringScribe.h>
#include <Inscription/ContainerSize.h>

#include "BinaryFixture.h"

class BinaryPolymorphicListFixture : public BinaryFixture
{
public:
    BinaryPolymorphicListFixture()
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
};

BinaryPolymorphicListFixture::Base::~Base()
{}

namespace Inscription
{
    template<>
    class Scribe<::BinaryPolymorphicListFixture::Base, BinaryArchive> :
        public CompositeScribe<::BinaryPolymorphicListFixture::Base, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };

    template<>
    class Scribe<::BinaryPolymorphicListFixture::Derived, BinaryArchive> :
        public CompositeScribe<::BinaryPolymorphicListFixture::Derived, BinaryArchive>
    {
    public:
        static TypeHandle PrincipleTypeHandle(const ArchiveT& archive);
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}

SCENARIO_METHOD
(
    BinaryPolymorphicListFixture,
    "loading list of polymorphic pointers in binary",
    "[binary][pointer][polymorphic]"
) {
    GIVEN("saved list of polymorphic pointers")
    {
        std::vector<Base*> savedOwning;
        {
            auto startingGroup = dataGeneration.RandomHeapGroup<Derived>(3);
            for (auto& loop : startingGroup)
                savedOwning.push_back(loop);
        }

        std::vector<Derived*> castedOwning;
        for (auto& loop : savedOwning)
            castedOwning.push_back(dynamic_cast<Derived*>(loop));

        {
            auto outputArchive = CreateRegistered<OutputArchive>();

            ::Inscription::ContainerSize containerSize(savedOwning.size());
            outputArchive(containerSize);
            for (auto& loop : savedOwning)
                outputArchive(loop);
        }

        WHEN("loading list of polymorphic pointers")
        {
            std::vector<Base*> loadedOwning;

            {
                auto inputArchive = CreateRegistered<InputArchive>();

                ::Inscription::ContainerSize containerSize;
                inputArchive(containerSize);
                while (containerSize-- > 0)
                {
                    Base* ptr = nullptr;
                    inputArchive(ptr);
                    loadedOwning.push_back(ptr);
                }
            }

            std::vector<Derived*> loadedCastedOwning;
            for (auto& loop : loadedOwning)
                loadedCastedOwning.push_back(dynamic_cast<Derived*>(loop));

            THEN("list of polymorphic pointers is valid")
            {
                REQUIRE(!loadedOwning.empty());
                REQUIRE(loadedOwning.size() == savedOwning.size());
                for (size_t i = 0; i < savedOwning.size(); ++i)
                {
                    auto savedOwningElement = savedOwning[i];
                    auto loadedOwningElement = loadedOwning[i];
                    auto savedCastedOwningElement = castedOwning[i];
                    auto loadedCastedOwningElement = loadedCastedOwning[i];

                    REQUIRE(savedOwningElement->BaseValue() == loadedOwningElement->BaseValue());
                    REQUIRE(savedCastedOwningElement->DerivedValue() == loadedCastedOwningElement->DerivedValue());
                }

                for (auto& loop : savedOwning)
                    delete loop;
                for (auto& loop : loadedOwning)
                    delete loop;
            }
        }
    }
}

namespace Inscription
{
    void Scribe<::BinaryPolymorphicListFixture::Base, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object.baseValue);
    }

    TypeHandle Scribe<::BinaryPolymorphicListFixture::Derived, BinaryArchive>::PrincipleTypeHandle(const ArchiveT& archive)
    {
        return "CustomConstructionDerived";
    }

    void Scribe<::BinaryPolymorphicListFixture::Derived, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive.BaseScriven<::BinaryPolymorphicListFixture::Base>(object);
        archive(object.derivedValue);
    }
}