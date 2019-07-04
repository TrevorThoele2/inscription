#include <boost/test/unit_test.hpp>

#include <Inscription/TableScribe.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/PointerScribe.h>

#include <Inscription/MemoryScribe.h>
#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"

class BinaryTableFixture : public BinaryFixture
{
public:
    BinaryTableFixture()
    {
        typeRegistrationContext.RegisterType<Base>();
        typeRegistrationContext.RegisterType<Derived>();
    }

    class Base
    {
    public:
        int baseValue;
    public:
        Base(int baseValue) : baseValue(baseValue)
        {}

        virtual ~Base() = 0;
    };

    class Derived : public Base
    {
    public:
        std::string derivedValue;
    public:
        Derived(int baseValue, const std::string& derivedValue) :
            Base(baseValue), derivedValue(derivedValue)
        {}
    };
};

BinaryTableFixture::Base::~Base()
{}

namespace Inscription
{
    template<>
    class TableData<::BinaryTableFixture::Base, BinaryArchive>
    {
    public:
        int baseValue;
    };

    template<>
    class Table<::BinaryTableFixture::Base, BinaryArchive> : public
        TableBase<::BinaryTableFixture::Base, BinaryArchive>
    {
    public:
        Table();
        void Construct(DataT& data, ObjectT*& object, ArchiveT& archive) override {}
    };

    template<>
    class Scribe<::BinaryTableFixture::Base, BinaryArchive> : public
        TableScribe<::BinaryTableFixture::Base, BinaryArchive>
    {};

    template<>
    class TableData<::BinaryTableFixture::Derived, BinaryArchive> : public
        TableData<::BinaryTableFixture::Base, BinaryArchive>
    {
    public:
        std::string derivedValue;
    };

    template<>
    class Table<::BinaryTableFixture::Derived, BinaryArchive> : public
        TableBase<::BinaryTableFixture::Derived, BinaryArchive>
    {
    public:
        Table();
        void Construct(DataT& data, ObjectT*& object, ArchiveT& archive) override;
    };

    template<>
    class Scribe<::BinaryTableFixture::Derived, BinaryArchive> : public
        TableScribe<::BinaryTableFixture::Derived, BinaryArchive>
    {
    public:
        static const ClassNameResolver classNameResolver;
    };
}

BOOST_FIXTURE_TEST_SUITE(BinaryPolymorphicConstructionTests,
    BinaryTableFixture)

BOOST_AUTO_TEST_CASE(Construction)
{
    Base* saved = dataGeneration.Generator<Derived>().RandomHeap<int, std::string>();
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

    BOOST_REQUIRE(loadedCasted != nullptr);
    BOOST_REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
    BOOST_REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);

    delete saved;
    delete loaded;
}

BOOST_AUTO_TEST_SUITE_END()

namespace Inscription
{
    Table<::BinaryTableFixture::Base, BinaryArchive>::Table()
    {
        MergeDataEntries({
            DataEntry::Auto(&ObjectT::baseValue, &DataT::baseValue) });
    }

    void Table<::BinaryTableFixture::Derived, BinaryArchive>::Construct(
        DataT& data, ObjectT*& object, ArchiveT& archive)
    {
        object = new ObjectT(
            data.baseValue,
            data.derivedValue);
    }

    Table<::BinaryTableFixture::Derived, BinaryArchive>::Table()
    {
        MergeDataEntries({
            DataEntry::Base(Type<::BinaryTableFixture::Base>{}),
            DataEntry::Auto(&ObjectT::derivedValue, &DataT::derivedValue) });
    }

    const Scribe<::BinaryTableFixture::Derived, BinaryArchive>::ClassNameResolver
        Scribe<::BinaryTableFixture::Derived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("Derived");
}