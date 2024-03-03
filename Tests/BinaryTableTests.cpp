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

BinaryTableFixture::Base::~Base()
{}

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

BinaryTableFixture::ObjectScrivenBase::~ObjectScrivenBase()
{}

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

BinaryTableFixture::TableConstructionBase::~TableConstructionBase()
{}

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

TEST_CASE_METHOD(BinaryTableFixture, "binary table")
{
    SECTION("default construction serialization")
    {
        DefaultConstructionDerived saved = dataGeneration.RandomStack<DefaultConstructionDerived, int, std::string>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        DefaultConstructionDerived loaded;

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);
        }

        REQUIRE(loaded.baseValue == saved.baseValue);
        REQUIRE(loaded.derivedValue == saved.derivedValue);
    }

    SECTION("DefaultConstruction construction")
    {
        Base* saved = dataGeneration.RandomHeap<DefaultConstructionDerived, int, std::string>();
        DefaultConstructionDerived* savedCasted = dynamic_cast<DefaultConstructionDerived*>(saved);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        Base* loaded = nullptr;

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);
        }

        DefaultConstructionDerived* loadedCasted = dynamic_cast<DefaultConstructionDerived*>(loaded);

        REQUIRE(loadedCasted != nullptr);
        REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
        REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);

        delete saved;
        delete loaded;
    }

    SECTION("CustomConstruction serialization")
    {
        CustomConstructionDerived saved = dataGeneration.RandomStack<CustomConstructionDerived, int, std::string>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        CustomConstructionDerived loaded(0, "");

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);
        }

        REQUIRE(loaded.baseValue == saved.baseValue);
        REQUIRE(loaded.derivedValue == saved.derivedValue);
    }

    SECTION("CustomConstruction construction")
    {
        Base* saved = dataGeneration.RandomHeap<CustomConstructionDerived, int, std::string>();
        CustomConstructionDerived* savedCasted = dynamic_cast<CustomConstructionDerived*>(saved);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        Base* loaded = nullptr;

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);
        }

        CustomConstructionDerived* loadedCasted = dynamic_cast<CustomConstructionDerived*>(loaded);

        REQUIRE(loadedCasted != nullptr);
        REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
        REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);

        delete saved;
        delete loaded;
    }

    SECTION("TableConstruction serialization")
    {
        TableConstructionDerived saved = dataGeneration.RandomStack<TableConstructionDerived, int, std::string>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        TableConstructionDerived loaded(0, "");

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);
        }

        REQUIRE(loaded.baseValue == saved.baseValue);
        REQUIRE(loaded.derivedValue == saved.derivedValue);
    }

    SECTION("TableConstruction construction")
    {
        TableConstructionBase* saved = dataGeneration.RandomHeap<TableConstructionDerived, int, std::string>();
        TableConstructionDerived* savedCasted = dynamic_cast<TableConstructionDerived*>(saved);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        TableConstructionBase* loaded = nullptr;

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);
        }

        TableConstructionDerived* loadedCasted = dynamic_cast<TableConstructionDerived*>(loaded);

        REQUIRE(loadedCasted != nullptr);
        REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
        REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);

        delete saved;
        delete loaded;
    }

    SECTION("ObjectScriven serialization")
    {
        ObjectScrivenDerived saved = dataGeneration.RandomStack<ObjectScrivenDerived, int, std::string>();
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

        REQUIRE(loaded.baseValue == saved.baseValue);
        REQUIRE(loaded.baseObjectScrivenValue == saved.baseObjectScrivenValue);
        REQUIRE(loaded.derivedValue == saved.derivedValue);
        REQUIRE(loaded.derivedObjectScrivenValue == saved.derivedObjectScrivenValue);
    }

    SECTION("ObjectScriven construction")
    {
        ObjectScrivenBase* saved = dataGeneration.RandomHeap<ObjectScrivenDerived, int, std::string>();
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

        REQUIRE(loadedCasted != nullptr);
        REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
        REQUIRE(loadedCasted->baseObjectScrivenValue == savedCasted->baseObjectScrivenValue);
        REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);
        REQUIRE(loadedCasted->derivedObjectScrivenValue == savedCasted->derivedObjectScrivenValue);

        delete saved;
        delete loaded;
    }

    SECTION("UsingEntries serialization")
    {
        UsingEntriesDerived saved = dataGeneration.RandomStack<UsingEntriesDerived, int, std::string>();

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        UsingEntriesDerived loaded(0, "");

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);
        }

        REQUIRE(loaded.baseValue == saved.baseValue);
        REQUIRE(loaded.derivedValue == saved.derivedValue);
    }

    SECTION("UsingEntries construction")
    {
        Base* saved = dataGeneration.RandomHeap<UsingEntriesDerived, int, std::string>();
        UsingEntriesDerived* savedCasted = dynamic_cast<UsingEntriesDerived*>(saved);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        Base* loaded = nullptr;

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);
        }

        UsingEntriesDerived* loadedCasted = dynamic_cast<UsingEntriesDerived*>(loaded);

        REQUIRE(loadedCasted != nullptr);
        REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
        REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);

        delete saved;
        delete loaded;
    }

    SECTION("UsingEntryPointer serialization")
    {
        UsingEntriesDerived savedSource = dataGeneration.RandomStack<UsingEntriesDerived, int, std::string>();
        UsingEntryPointerDerived saved(dataGeneration.Random<int>(), &savedSource.derivedValue);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(savedSource);
            outputArchive(saved);
        }

        UsingEntriesDerived loadedSource(0, "");
        UsingEntryPointerDerived loaded(0, nullptr);

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loadedSource);
            inputArchive(loaded);
        }

        REQUIRE(loaded.baseValue == saved.baseValue);
        REQUIRE(loaded.derivedValue == &loadedSource.derivedValue);
    }

    SECTION("UsingEntryPointer construction")
    {
        auto savedSource = dataGeneration.RandomStack<UsingEntriesDerived, int, std::string>();

        Base* saved = new UsingEntryPointerDerived(dataGeneration.Random<int>(), &savedSource.derivedValue);
        UsingEntryPointerDerived* savedCasted = dynamic_cast<UsingEntryPointerDerived*>(saved);

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(savedSource);
            outputArchive(saved);
        }

        UsingEntriesDerived loadedSource(0, "");
        Base* loaded = nullptr;

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loadedSource);
            inputArchive(loaded);
        }

        UsingEntryPointerDerived* loadedCasted = dynamic_cast<UsingEntryPointerDerived*>(loaded);

        REQUIRE(loadedCasted != nullptr);
        REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
        REQUIRE(loadedCasted->derivedValue == &loadedSource.derivedValue);

        delete saved;
        delete loaded;
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

    void Scribe<::BinaryTableFixture::Base, BinaryArchive>::Table::ConstructImplementation(
        ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }

    Scribe<::BinaryTableFixture::DefaultConstructionDerived, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Base(data.base),
            DataLink::Auto(&ObjectT::derivedValue, &DataT::derivedValue) }
        );
    }

    void Scribe<::BinaryTableFixture::DefaultConstructionDerived, BinaryArchive>::Table::ConstructImplementation(
        ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }

    const Scribe<::BinaryTableFixture::DefaultConstructionDerived, BinaryArchive>::ClassNameResolver
        Scribe<::BinaryTableFixture::DefaultConstructionDerived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("DefaultConstructionDerived");

    Scribe<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Base(Type<::BinaryTableFixture::Base>{}),
            DataLink::Auto(&ObjectT::derivedValue, &DataT::derivedValue) }
        );
    }

    void Scribe<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive>::Table::ConstructImplementation(
        ObjectT* storage, ArchiveT& archive)
    {
        new (storage) ObjectT(data.base->baseValue, data.derivedValue);
    }

    const Scribe<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive>::ClassNameResolver
        Scribe<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("CustomConstructionDerived");

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

    void Scribe<::BinaryTableFixture::ObjectScrivenBase, BinaryArchive>::Table::ConstructImplementation(
        ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }

    Scribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Base(Type<::BinaryTableFixture::ObjectScrivenBase>{}),
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

    void Scribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>::Table::ConstructImplementation(
        ObjectT* storage, ArchiveT& archive)
    {

    }

    const Scribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>::ClassNameResolver
        Scribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("ObjectScrivenDerived");

    Scribe<::BinaryTableFixture::TableConstructionBase, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Auto(&ObjectT::baseValue, &DataT::baseValue) }
        );
    }

    void Scribe<::BinaryTableFixture::TableConstructionBase, BinaryArchive>::Table::ConstructImplementation(
        ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }

    Scribe<::BinaryTableFixture::TableConstructionDerived, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Base(Type<::BinaryTableFixture::TableConstructionBase>{}),
            DataLink::Auto(&ObjectT::derivedValue, &DataT::derivedValue) }
        );
    }

    void Scribe<::BinaryTableFixture::TableConstructionDerived, BinaryArchive>::Table::ConstructImplementation(
        ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }

    const Scribe<::BinaryTableFixture::TableConstructionDerived, BinaryArchive>::ClassNameResolver
        Scribe<::BinaryTableFixture::TableConstructionDerived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("TableConstructionDerived");

    Scribe<::BinaryTableFixture::UsingEntriesDerived, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Base(data.base),
            DataLink::Auto(&ObjectT::derivedValue, &DataT::derivedValue) }
        );
    }

    void Scribe<::BinaryTableFixture::UsingEntriesDerived, BinaryArchive>::Table::ConstructImplementation(
        ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }

    const Scribe<::BinaryTableFixture::UsingEntriesDerived, BinaryArchive>::ClassNameResolver
        Scribe<::BinaryTableFixture::UsingEntriesDerived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("UsingEntriesDerived");

    Scribe<::BinaryTableFixture::UsingEntryPointerDerived, BinaryArchive>::Table::Table()
    {
        MergeDataLinks
        ({
            DataLink::Base(data.base),
            DataLink::Auto(&ObjectT::derivedValue, &DataT::derivedValue) }
        );
    }

    void Scribe<::BinaryTableFixture::UsingEntryPointerDerived, BinaryArchive>::Table::ConstructImplementation(
        ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }

    const Scribe<::BinaryTableFixture::UsingEntryPointerDerived, BinaryArchive>::ClassNameResolver
        Scribe<::BinaryTableFixture::UsingEntryPointerDerived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("UsingEntryPointerDerived");

    Scribe<::BinaryTableFixture::NonDefault, BinaryArchive>::Table::Table()
    {
        MergeDataLinks({
            DataLink::Auto(&ObjectT::value, &DataT::value) }
        );
    }

    void Scribe<::BinaryTableFixture::NonDefault, BinaryArchive>::Table::ConstructImplementation(
        ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}