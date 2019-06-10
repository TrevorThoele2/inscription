#include <boost/test/unit_test.hpp>

#include <Inscription/Numeric.h>
#include <Inscription/Pointer.h>
#include <Inscription/String.h>
#include <Inscription/ContainerSize.h>

#include "BinaryFixture.h"

class ListPolymorphicBinarySerializationTestsFixture : public BinaryFixture
{
public:
    ListPolymorphicBinarySerializationTestsFixture()
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

ListPolymorphicBinarySerializationTestsFixture::Base::~Base()
{}

namespace Inscription
{
    template<>
    class Scribe<::ListPolymorphicBinarySerializationTestsFixture::Base, BinaryArchive> :
        public CompositeScribe<::ListPolymorphicBinarySerializationTestsFixture::Base, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };

    template<>
    class Scribe<::ListPolymorphicBinarySerializationTestsFixture::Derived, BinaryArchive> :
        public CompositeScribe<::ListPolymorphicBinarySerializationTestsFixture::Derived, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
        static const ClassNameResolver classNameResolver;
    };
}

BOOST_FIXTURE_TEST_SUITE(ListPolymorphicBinarySerializationTests, ListPolymorphicBinarySerializationTestsFixture)

BOOST_AUTO_TEST_CASE(PolymorphicOwningPointer_SavesAndLoads)
{
    std::vector<Base*> savedOwning;
    {
        auto startingGroup = dataGeneration.Generator<Derived>().RandomHeapGroup(3);
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

    BOOST_REQUIRE(!loadedOwning.empty());
    BOOST_REQUIRE(loadedOwning.size() == savedOwning.size());
    for (size_t i = 0; i < savedOwning.size(); ++i)
    {
        auto savedOwningElement = savedOwning[i];
        auto loadedOwningElement = loadedOwning[i];
        auto savedCastedOwningElement = castedOwning[i];
        auto loadedCastedOwningElement = loadedCastedOwning[i];

        BOOST_REQUIRE(savedOwningElement->BaseValue() == loadedOwningElement->BaseValue());
        BOOST_REQUIRE(savedCastedOwningElement->DerivedValue() == loadedCastedOwningElement->DerivedValue());
    }

    for (auto& loop : savedOwning)
        delete loop;
    for (auto& loop : loadedOwning)
        delete loop;
}

BOOST_AUTO_TEST_SUITE_END()

namespace Inscription
{
    void Scribe<::ListPolymorphicBinarySerializationTestsFixture::Base, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        archive(object.baseValue);
    }

    void Scribe<::ListPolymorphicBinarySerializationTestsFixture::Derived, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<::ListPolymorphicBinarySerializationTestsFixture::Base>(object, archive);
        archive(object.derivedValue);
    }

    const Scribe<::ListPolymorphicBinarySerializationTestsFixture::Derived, BinaryArchive>::ClassNameResolver
        Scribe<::ListPolymorphicBinarySerializationTestsFixture::Derived, BinaryArchive>::classNameResolver = CreateSingleNameResolver("Derived");
}