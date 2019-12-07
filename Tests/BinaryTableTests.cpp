#include <catch.hpp>

#include "BinaryTableTests.h"

#include <Inscription/TableScribe.h>
#include <Inscription/NumericScribe.h>
#include <Inscription/PointerScribe.h>

#include <Inscription/MemoryScribe.h>
#include <Inscription/StringScribe.h>

BinaryTableFixture::BinaryTableFixture()
{
    typeRegistrationContext.RegisterType<Base>();
    typeRegistrationContext.RegisterType<CustomConstructionDerived>();

    typeRegistrationContext.RegisterType<ObjectScrivenBase>();
    typeRegistrationContext.RegisterType<ObjectScrivenDerived>();

    typeRegistrationContext.RegisterType<DefaultConstructionDerived>();

    typeRegistrationContext.RegisterType<TableConstructionBase>();
    typeRegistrationContext.RegisterType<TableConstructionDerived>();

    typeRegistrationContext.RegisterType<UsingEntriesDerived>();
    typeRegistrationContext.RegisterType<UsingEntryPointerDerived>();
}

class BinaryTableFixture::Base
{
public:
    int baseValue;
public:
    Base(int baseValue) : baseValue(baseValue)
    {}

    Base(const ::Inscription::BinaryTableData<Base>& data) : baseValue(data.baseValue)
    {}

    virtual ~Base() = 0;
};

BinaryTableFixture::Base::~Base() = default;

class BinaryTableFixture::CustomConstructionDerived : public Base
{
public:
    std::string derivedValue;
public:
    CustomConstructionDerived(int baseValue, const std::string& derivedValue) :
        Base(baseValue), derivedValue(derivedValue)
    {}
};

class BinaryTableFixture::ObjectScrivenBase
{
public:
    int baseValue;
    short baseObjectScrivenValue = 0;
public:
    ObjectScrivenBase(int baseValue) : baseValue(baseValue)
    {}

    virtual ~ObjectScrivenBase() = 0;
};

BinaryTableFixture::ObjectScrivenBase::~ObjectScrivenBase() = default;

class BinaryTableFixture::ObjectScrivenDerived : public ObjectScrivenBase
{
public:
    std::string derivedValue;
    long long derivedObjectScrivenValue = 0;
public:
    ObjectScrivenDerived(int baseValue, const std::string& derivedValue) :
        ObjectScrivenBase(baseValue), derivedValue(derivedValue)
    {}
};

class BinaryTableFixture::DefaultConstructionDerived : public Base
{
public:
    std::string derivedValue;
public:
    DefaultConstructionDerived() : Base(0)
    {}

    DefaultConstructionDerived(int baseValue, const std::string& derivedValue) :
        Base(baseValue), derivedValue(derivedValue)
    {}
};

class BinaryTableFixture::TableConstructionBase
{
public:
    int baseValue;
public:
    TableConstructionBase(int baseValue) :
        baseValue(baseValue)
    {}

    TableConstructionBase(const ::Inscription::BinaryTableData<TableConstructionBase>& data) :
        baseValue(data.baseValue)
    {}

    virtual ~TableConstructionBase() = 0;
};

BinaryTableFixture::TableConstructionBase::~TableConstructionBase() = default;

class BinaryTableFixture::TableConstructionDerived : public BinaryTableFixture::TableConstructionBase
{
public:
    std::string derivedValue;
public:
    TableConstructionDerived(int baseValue, const std::string& derivedValue) :
        TableConstructionBase(baseValue), derivedValue(derivedValue)
    {}

    TableConstructionDerived(const ::Inscription::BinaryTableData<TableConstructionDerived>& data) :
        TableConstructionBase(data.base), derivedValue(data.derivedValue)
    {}
};

class BinaryTableFixture::UsingEntriesDerived : public BinaryTableFixture::Base
{
public:
    NonDefault derivedValue;
public:
    UsingEntriesDerived(int baseValue, const std::string& derivedValue) :
        Base(baseValue), derivedValue(derivedValue)
    {}

    UsingEntriesDerived(const ::Inscription::BinaryTableData<UsingEntriesDerived>& data) :
        Base(data.base), derivedValue(data.derivedValue)
    {}
};

class BinaryTableFixture::UsingEntryPointerDerived : public BinaryTableFixture::Base
{
public:
    NonDefault* derivedValue;
public:
    UsingEntryPointerDerived(int baseValue, NonDefault* derivedValue) :
        Base(baseValue), derivedValue(derivedValue)
    {}

    UsingEntryPointerDerived(const ::Inscription::BinaryTableData<UsingEntryPointerDerived>& data) :
        Base(data.base), derivedValue(data.derivedValue)
    {}
};

BinaryTableFixture::NonDefault::NonDefault(const ::Inscription::BinaryTableData<NonDefault>& data) :
    value(data.value)
{}

SCENARIO_METHOD(BinaryTableFixture, "loading binary table", "[binary][table]")
{
    GIVEN("saved default construction object")
    {
        DefaultConstructionDerived saved = dataGeneration.RandomStack<DefaultConstructionDerived, int, std::string>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading default construction object")
        {
            DefaultConstructionDerived loaded;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loaded);
            }

            THEN("is valid")
            {
                REQUIRE(loaded.baseValue == saved.baseValue);
                REQUIRE(loaded.derivedValue == saved.derivedValue);
            }
        }
    }

    GIVEN("saved default construction pointer")
    {
        Base* saved = dataGeneration.RandomHeap<DefaultConstructionDerived, int, std::string>();
        DefaultConstructionDerived* savedCasted = dynamic_cast<DefaultConstructionDerived*>(saved);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading default construction pointer")
        {
            Base* loaded = nullptr;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loaded);
            }

            THEN("is valid")
            {
                DefaultConstructionDerived* loadedCasted = dynamic_cast<DefaultConstructionDerived*>(loaded);
                REQUIRE(loadedCasted != nullptr);
                REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
                REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);

                delete saved;
                delete loaded;
            }
        }
    }

    GIVEN("saved custom construction object")
    {
        CustomConstructionDerived saved = dataGeneration.RandomStack<CustomConstructionDerived, int, std::string>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
        {
            CustomConstructionDerived loaded(0, "");

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loaded);
            }

            THEN("is valid")
            {
                REQUIRE(loaded.baseValue == saved.baseValue);
                REQUIRE(loaded.derivedValue == saved.derivedValue);
            }
        }
    }

    GIVEN("saved custom construction pointer")
    {
        Base* saved = dataGeneration.RandomHeap<CustomConstructionDerived, int, std::string>();
        CustomConstructionDerived* savedCasted = dynamic_cast<CustomConstructionDerived*>(saved);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
        {
            Base* loaded = nullptr;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loaded);
            }

            THEN("is valid")
            {
                CustomConstructionDerived* loadedCasted = dynamic_cast<CustomConstructionDerived*>(loaded);
                REQUIRE(loadedCasted != nullptr);
                REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
                REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);

                delete saved;
                delete loaded;
            }
        }
    }

    GIVEN("saved table construction override object")
    {
        TableConstructionDerived saved = dataGeneration.RandomStack<TableConstructionDerived, int, std::string>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
        {
            TableConstructionDerived loaded(0, "");

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loaded);
            }

            THEN("is valid")
            {
                REQUIRE(loaded.baseValue == saved.baseValue);
                REQUIRE(loaded.derivedValue == saved.derivedValue);
            }
        }
    }

    SECTION("saved table construction override pointer")
    {
        TableConstructionBase* saved = dataGeneration.RandomHeap<TableConstructionDerived, int, std::string>();
        TableConstructionDerived* savedCasted = dynamic_cast<TableConstructionDerived*>(saved);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
        {
            TableConstructionBase* loaded = nullptr;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loaded);
            }

            THEN("is valid")
            {
                TableConstructionDerived* loadedCasted = dynamic_cast<TableConstructionDerived*>(loaded);
                REQUIRE(loadedCasted != nullptr);
                REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
                REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);

                delete saved;
                delete loaded;
            }
        }
    }

    GIVEN("saved ObjectScriven overidden object")
    {
        ObjectScrivenDerived saved = dataGeneration.RandomStack<ObjectScrivenDerived, int, std::string>();
        saved.baseObjectScrivenValue = dataGeneration.Generator<short>().Random();
        saved.derivedObjectScrivenValue = dataGeneration.Generator<long long>().Random();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
        {
            ObjectScrivenDerived loaded(0, "");

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loaded);
            }

            THEN("is valid")
            {
                REQUIRE(loaded.baseValue == saved.baseValue);
                REQUIRE(loaded.baseObjectScrivenValue == saved.baseObjectScrivenValue);
                REQUIRE(loaded.derivedValue == saved.derivedValue);
                REQUIRE(loaded.derivedObjectScrivenValue == saved.derivedObjectScrivenValue);
            }
        }
    }

    GIVEN("saved ObjectScriven overidden pointer")
    {
        ObjectScrivenBase* saved = dataGeneration.RandomHeap<ObjectScrivenDerived, int, std::string>();
        ObjectScrivenDerived* savedCasted = dynamic_cast<ObjectScrivenDerived*>(saved);

        savedCasted->baseObjectScrivenValue = dataGeneration.Generator<short>().Random();
        savedCasted->derivedObjectScrivenValue = dataGeneration.Generator<long long>().Random();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
        {
            ObjectScrivenBase* loaded = nullptr;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loaded);
            }

            THEN("is valid")
            {
                ObjectScrivenDerived* loadedCasted = dynamic_cast<ObjectScrivenDerived*>(loaded);
                REQUIRE(loadedCasted != nullptr);
                REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
                REQUIRE(loadedCasted->baseObjectScrivenValue == savedCasted->baseObjectScrivenValue);
                REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);
                REQUIRE(loadedCasted->derivedObjectScrivenValue == savedCasted->derivedObjectScrivenValue);

                delete saved;
                delete loaded;
            }
        }
    }

    GIVEN("saved object using entries")
    {
        UsingEntriesDerived saved = dataGeneration.RandomStack<UsingEntriesDerived, int, std::string>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
        {
            UsingEntriesDerived loaded(0, "");

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loaded);
            }

            THEN("is valid")
            {
                REQUIRE(loaded.baseValue == saved.baseValue);
                REQUIRE(loaded.derivedValue == saved.derivedValue);
            }
        }
    }

    SECTION("saved object using entries through pointer")
    {
        Base* saved = dataGeneration.RandomHeap<UsingEntriesDerived, int, std::string>();
        UsingEntriesDerived* savedCasted = dynamic_cast<UsingEntriesDerived*>(saved);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading")
        {
            Base* loaded = nullptr;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loaded);
            }

            THEN("is valid")
            {
                UsingEntriesDerived* loadedCasted = dynamic_cast<UsingEntriesDerived*>(loaded);
                REQUIRE(loadedCasted != nullptr);
                REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
                REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);

                delete saved;
                delete loaded;
            }
        }
    }

    SECTION("saved object with entries pointing to data represented through entries")
    {
        UsingEntriesDerived savedSource = dataGeneration.RandomStack<UsingEntriesDerived, int, std::string>();
        UsingEntryPointerDerived saved(dataGeneration.Random<int>(), &savedSource.derivedValue);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(savedSource);
            outputArchive(saved);
        }

        WHEN("loading")
        {
            UsingEntriesDerived loadedSource(0, "");
            UsingEntryPointerDerived loaded(0, nullptr);

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loadedSource);
                inputArchive(loaded);
            }

            THEN("is valid")
            {
                REQUIRE(loaded.baseValue == saved.baseValue);
                REQUIRE(loaded.derivedValue == &loadedSource.derivedValue);
            }
        }
    }

    SECTION("saved object with entries pointing to data represented through entries as pointer")
    {
        auto savedSource = dataGeneration.RandomStack<UsingEntriesDerived, int, std::string>();

        Base* saved = new UsingEntryPointerDerived(dataGeneration.Random<int>(), &savedSource.derivedValue);
        UsingEntryPointerDerived* savedCasted = dynamic_cast<UsingEntryPointerDerived*>(saved);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(savedSource);
            outputArchive(saved);
        }

        WHEN("loading")
        {
            UsingEntriesDerived loadedSource(0, "");
            Base* loaded = nullptr;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loadedSource);
                inputArchive(loaded);
            }

            THEN("is valid")
            {
                UsingEntryPointerDerived* loadedCasted = dynamic_cast<UsingEntryPointerDerived*>(loaded);
                REQUIRE(loadedCasted != nullptr);
                REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
                REQUIRE(loadedCasted->derivedValue == &loadedSource.derivedValue);

                delete saved;
                delete loaded;
            }
        }
    }
}

namespace Inscription
{
    Scribe<::BinaryTableFixture::Base, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Auto(&ObjectT::baseValue, &DataT::baseValue) }
        );
    }

    Scribe<::BinaryTableFixture::DefaultConstructionDerived, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Base(data.base),
            DataLink::Auto(&ObjectT::derivedValue, &DataT::derivedValue) }
        );
    }

    Type Scribe<::BinaryTableFixture::DefaultConstructionDerived, BinaryArchive>::OutputType(
        const ArchiveT& archive
    ) {
        return "DefaultConstructionDerived";
    }

    Scribe<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Base(TypeIdentity<::BinaryTableFixture::Base>{}),
            DataLink::Auto(&ObjectT::derivedValue, &DataT::derivedValue) }
        );
    }

    void Scribe<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive>::Table::Construct(
        ObjectT* storage, ArchiveT& archive)
    {
        new (storage) ObjectT(data.base->baseValue, data.derivedValue);
    }

    Type Scribe<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive>::OutputType(
        const ArchiveT& archive
    ) {
        return "CustomConstructionDerived";
    }

    Scribe<::BinaryTableFixture::ObjectScrivenBase, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Auto(&ObjectT::baseValue, &DataT::baseValue) }
        );
    }

    void Scribe<::BinaryTableFixture::ObjectScrivenBase, BinaryArchive>::Table::ObjectScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object.baseObjectScrivenValue);
    }

    Scribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Base(TypeIdentity<::BinaryTableFixture::ObjectScrivenBase>{}),
            DataLink::Auto(&ObjectT::derivedValue, &DataT::derivedValue) }
        );
    }

    void Scribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>::Table::Construct(
        ObjectT* storage, ArchiveT& archive)
    {
        new (storage) ObjectT(data.base->baseValue, data.derivedValue);
    }

    void Scribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>::Table::ObjectScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        archive(object.derivedObjectScrivenValue);
    }

    Type Scribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>::OutputType(
        const ArchiveT& archive
    ) {
        return "ObjectScrivenDerived";
    }

    Scribe<::BinaryTableFixture::TableConstructionBase, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Auto(&ObjectT::baseValue, &DataT::baseValue) }
        );
    }

    Scribe<::BinaryTableFixture::TableConstructionDerived, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Base(TypeIdentity<::BinaryTableFixture::TableConstructionBase>{}),
            DataLink::Auto(&ObjectT::derivedValue, &DataT::derivedValue) }
        );
    }

    Type Scribe<::BinaryTableFixture::TableConstructionDerived, BinaryArchive>::OutputType(
        const ArchiveT& archive
    ) {
        return "TableConstructionDerived";
    }

    Scribe<::BinaryTableFixture::UsingEntriesDerived, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Base(data.base),
            DataLink::Auto(&ObjectT::derivedValue, &DataT::derivedValue) }
        );
    }

    Type Scribe<::BinaryTableFixture::UsingEntriesDerived, BinaryArchive>::OutputType(
        const ArchiveT& archive
    ) {
        return "UsingEntriesDerived";
    }

    Scribe<::BinaryTableFixture::UsingEntryPointerDerived, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Base(data.base),
            DataLink::Auto(&ObjectT::derivedValue, &DataT::derivedValue) }
        );
    }

    Type Scribe<::BinaryTableFixture::UsingEntryPointerDerived, BinaryArchive>::OutputType(
        const ArchiveT& archive
    ) {
        return "UsingEntryPointerDerived";
    }

    Scribe<::BinaryTableFixture::NonDefault, BinaryArchive>::Table::Table()
    {
        MergeDataLinks({
            DataLink::Auto(&ObjectT::value, &DataT::value) }
        );
    }
}