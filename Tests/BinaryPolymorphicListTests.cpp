#include <catch.hpp>

#include <Inscription/CompositeScribeCategory.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/PointerScribe.h>
#include <Inscription/StringScribe.h>
#include <Inscription/ContainerSize.h>

#include "GeneralFixture.h"

class BinaryPolymorphicListFixture : public GeneralFixture
{
public:
    Inscription::TypeRegistrationContext<Inscription::Format::Binary> typeRegistrationContext;

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
        void Scriven(ObjectT& object, Format::Binary& format);
    };

    template<class Format>
    struct ScribeTraits<BinaryPolymorphicListFixture::Base, Format> final
    {
        using Category = CompositeScribeCategory<BinaryPolymorphicListFixture::Base>;
    };

    template<>
    class Scribe<BinaryPolymorphicListFixture::Derived> final
    {
    public:
        using ObjectT = BinaryPolymorphicListFixture::Derived;
    public:
        static Type OutputType(const Format::Binary& format);
    public:
        void Scriven(ObjectT& object, Format::Binary& format);
    };

    template<class Format>
    struct ScribeTraits<BinaryPolymorphicListFixture::Derived, Format> final
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
            auto file = Inscription::File::OutputBinary("Test.dat");
            auto archive = Inscription::Archive::OutputBinary(file);
            auto format = Inscription::Format::OutputBinary(archive, typeRegistrationContext);

            Inscription::ContainerSize containerSize(savedOwning.size());
            format(containerSize);
            for (auto& loop : savedOwning)
                format(loop);
        }

        WHEN("loading list of polymorphic pointers")
        {
            std::vector<Base*> loadedOwning;

            {
                auto file = Inscription::File::InputBinary("Test.dat");
                auto archive = Inscription::Archive::InputBinary(file);
                auto format = Inscription::Format::InputBinary(archive, typeRegistrationContext);

                Inscription::ContainerSize containerSize;
                format(containerSize);
                while (containerSize-- > 0)
                {
                    Base* ptr = nullptr;
                    format(ptr);
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
    void Scribe<BinaryPolymorphicListFixture::Base>::Scriven(ObjectT& object, Format::Binary& format)
    {
        format(object.baseValue);
    }

    Type Scribe<BinaryPolymorphicListFixture::Derived>::OutputType(const Format::Binary& format)
    {
        return "CustomConstructionDerived";
    }

    void Scribe<BinaryPolymorphicListFixture::Derived>::Scriven(ObjectT& object, Format::Binary& format)
    {
        BaseScriven<BinaryPolymorphicListFixture::Base>(object, format);
        format(object.derivedValue);
    }
}