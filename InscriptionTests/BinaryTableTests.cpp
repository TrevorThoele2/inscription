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

        typeRegistrationContext.RegisterType<ObjectScrivenBase>();
        typeRegistrationContext.RegisterType<ObjectScrivenDerived>();
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

    class ObjectScrivenBase
    {
    public:
        int baseValue;
        short baseObjectScrivenValue = 0;
    public:
        ObjectScrivenBase(int baseValue) : baseValue(baseValue)
        {}

        virtual ~ObjectScrivenBase() = 0;
    };

    class ObjectScrivenDerived : public ObjectScrivenBase
    {
    public:
        std::string derivedValue;
        long long derivedObjectScrivenValue = 0;
    public:
        ObjectScrivenDerived(int baseValue, const std::string& derivedValue) :
            ObjectScrivenBase(baseValue), derivedValue(derivedValue)
        {}
    };
};

BinaryTableFixture::Base::~Base()
{}

BinaryTableFixture::ObjectScrivenBase::~ObjectScrivenBase()
{}

namespace Inscription
{
    template<>
    class Scribe<::BinaryTableFixture::Base, BinaryArchive> : public
        TableScribe<::BinaryTableFixture::Base, BinaryArchive>
    {
    public:
        struct Data
        {
            int baseValue;
        };

        class Table : public TableBase<Data>
        {
        public:
            Table();
        };
    };

    template<>
    class Scribe<::BinaryTableFixture::Derived, BinaryArchive> : public
        TableScribe<::BinaryTableFixture::Derived, BinaryArchive>
    {
    public:
        struct Data
        {
            std::string derivedValue;
        };

        class Table : public TableBase<Data>
        {
        public:
            Table();
        protected:
            void ConstructImplementation(DataT& data, ObjectT*& object, ArchiveT& archive) override;
        };
    public:
        static const ClassNameResolver classNameResolver;
    };

    template<>
    class Scribe<::BinaryTableFixture::ObjectScrivenBase, BinaryArchive> : public
        TableScribe<::BinaryTableFixture::ObjectScrivenBase, BinaryArchive>
    {
    public:
        struct Data
        {
            int baseValue;
        };

        class Table : public TableBase<Data>
        {
        public:
            Table();
        protected:
            void ObjectScrivenImplementation(DataT& data, ObjectT& object, ArchiveT& archive) override;
        };
    };

    template<>
    class Scribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive> : public
        TableScribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>
    {
    public:
        struct Data
        {
            std::string derivedValue;
        };

        class Table : public TableBase<Data>
        {
        public:
            Table();
        protected:
            void ObjectScrivenImplementation(DataT& data, ObjectT& object, ArchiveT& archive) override;
            void ConstructImplementation(DataT& data, ObjectT*& object, ArchiveT& archive) override;
        };
    public:
        static const ClassNameResolver classNameResolver;
    };
}

BOOST_FIXTURE_TEST_SUITE(BinaryTableTests, BinaryTableFixture)

BOOST_AUTO_TEST_CASE(Serialization)
{
    Derived saved = dataGeneration.Generator<Derived>().RandomStack<int, std::string>();

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(saved);
    }

    Derived loaded(0, "");

    {
        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loaded);
    }

    BOOST_REQUIRE(loaded.baseValue == saved.baseValue);
    BOOST_REQUIRE(loaded.derivedValue == saved.derivedValue);
}

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

BOOST_AUTO_TEST_CASE(ObjectScrivenSerialization)
{
    ObjectScrivenDerived saved = dataGeneration.Generator<ObjectScrivenDerived>().RandomStack<int, std::string>();
    saved.baseObjectScrivenValue = dataGeneration.Generator<short>().Random();
    saved.derivedObjectScrivenValue = dataGeneration.Generator<long long>().Random();

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(saved);
    }

    ObjectScrivenDerived loaded(0, "");

    {
        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loaded);
    }

    BOOST_REQUIRE(loaded.baseValue == saved.baseValue);
    BOOST_REQUIRE(loaded.baseObjectScrivenValue == saved.baseObjectScrivenValue);
    BOOST_REQUIRE(loaded.derivedValue == saved.derivedValue);
    BOOST_REQUIRE(loaded.derivedObjectScrivenValue == saved.derivedObjectScrivenValue);
}

BOOST_AUTO_TEST_CASE(ObjectScrivenConstruction)
{
    ObjectScrivenBase* saved = dataGeneration.Generator<ObjectScrivenDerived>().RandomHeap<int, std::string>();
    ObjectScrivenDerived* savedCasted = dynamic_cast<ObjectScrivenDerived*>(saved);

    savedCasted->baseObjectScrivenValue = dataGeneration.Generator<short>().Random();
    savedCasted->derivedObjectScrivenValue = dataGeneration.Generator<long long>().Random();

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(saved);
    }

    ObjectScrivenBase* loaded = nullptr;

    {
        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loaded);
    }

    ObjectScrivenDerived* loadedCasted = dynamic_cast<ObjectScrivenDerived*>(loaded);

    BOOST_REQUIRE(loadedCasted != nullptr);
    BOOST_REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
    BOOST_REQUIRE(loadedCasted->baseObjectScrivenValue == savedCasted->baseObjectScrivenValue);
    BOOST_REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);
    BOOST_REQUIRE(loadedCasted->derivedObjectScrivenValue == savedCasted->derivedObjectScrivenValue);

    delete saved;
    delete loaded;
}

BOOST_AUTO_TEST_SUITE_END()

namespace Inscription
{
    Scribe<::BinaryTableFixture::Base, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Auto(&ObjectT::baseValue, &DataT::baseValue) });
    }

    void Scribe<::BinaryTableFixture::Derived, BinaryArchive>::Table::ConstructImplementation(
        DataT& data, ObjectT*& object, ArchiveT& archive)
    {
        auto baseData = AsAutoBase<::BinaryTableFixture::Base>();
        object = new ObjectT(
            baseData->baseValue,
            data.derivedValue);
    }

    Scribe<::BinaryTableFixture::Derived, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Base(Type<::BinaryTableFixture::Base>{}),
            DataEntry::Auto(&ObjectT::derivedValue, &DataT::derivedValue) });
    }

    const Scribe<::BinaryTableFixture::Derived, BinaryArchive>::ClassNameResolver
        Scribe<::BinaryTableFixture::Derived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("Derived");

    Scribe<::BinaryTableFixture::ObjectScrivenBase, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Auto(&ObjectT::baseValue, &DataT::baseValue) });
    }

    void Scribe<::BinaryTableFixture::ObjectScrivenBase, BinaryArchive>::Table::
        ObjectScrivenImplementation(DataT& data, ObjectT& object, ArchiveT& archive)
    {
        archive(object.baseObjectScrivenValue);
    }

    void Scribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>::Table::
        ObjectScrivenImplementation(DataT& data, ObjectT& object, ArchiveT& archive)
    {
        archive(object.derivedObjectScrivenValue);
    }

    void Scribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>::Table::
        ConstructImplementation(DataT& data, ObjectT*& object, ArchiveT& archive)
    {
        auto baseData = AsAutoBase<::BinaryTableFixture::ObjectScrivenBase>();
        object = new ObjectT(
            baseData->baseValue,
            data.derivedValue);
    }

    Scribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Base(Type<::BinaryTableFixture::ObjectScrivenBase>{}),
            DataEntry::Auto(&ObjectT::derivedValue, &DataT::derivedValue) });
    }

    const Scribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>::ClassNameResolver
        Scribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("ObjectScrivenDerived");
}