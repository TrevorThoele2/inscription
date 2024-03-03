#include <catch.hpp>

#include <Inscription/CompositeScribeCategory.h>
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

BinaryPolymorphicListFixture::Base::~Base() = default;

namespace Inscription
{
    template<>
    class Scribe<BinaryPolymorphicListFixture::Base> final
    {
    public:
        using ObjectT = BinaryPolymorphicListFixture::Base;
    public:
        void Scriven(ObjectT& object, Archive::Binary& archive);
    };

    template<class Archive>
    struct ScribeTraits<BinaryPolymorphicListFixture::Base, Archive> final
    {
        using Category = CompositeScribeCategory<BinaryPolymorphicListFixture::Base>;
    };

    template<>
    class Scribe<BinaryPolymorphicListFixture::Derived> final
    {
    public:
        using ObjectT = BinaryPolymorphicListFixture::Derived;
    public:
        static Type OutputType(const Archive::Binary& archive);
    public:
        void Scriven(ObjectT& object, Archive::Binary& archive);
    };

    template<class Archive>
    struct ScribeTraits<BinaryPolymorphicListFixture::Derived, Archive> final
    {
        using Category = CompositeScribeCategory<BinaryPolymorphicListFixture::Derived>;
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
    void Scribe<BinaryPolymorphicListFixture::Base>::Scriven(ObjectT& object, Archive::Binary& archive)
    {
        archive(object.baseValue);
    }

    Type Scribe<BinaryPolymorphicListFixture::Derived>::OutputType(const Archive::Binary& archive)
    {
        return "CustomConstructionDerived";
    }

    void Scribe<BinaryPolymorphicListFixture::Derived>::Scriven(ObjectT& object, Archive::Binary& archive)
    {
        BaseScriven<BinaryPolymorphicListFixture::Base>(object, archive);
        archive(object.derivedValue);
    }
}