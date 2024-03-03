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
    BinaryTableFixture();

    class Base;
    class CustomConstructionDerived;

    class ObjectScrivenBase;
    class ObjectScrivenDerived;

    class DefaultConstructionDerived;

    class TableConstructionBase;
    class TableConstructionDerived;
};

namespace Inscription
{
    template<>
    struct TableData<::BinaryTableFixture::Base, BinaryArchive> :
        public TableDataBase<::BinaryTableFixture::Base, BinaryArchive>
    {
        int baseValue;
    };

    template<>
    class Scribe<::BinaryTableFixture::Base, BinaryArchive> : public
        TableScribe<::BinaryTableFixture::Base, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        protected:
            void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
            {
                DoBasicConstruction(storage, archive);
            }
        };
    };

    template<>
    struct TableData<::BinaryTableFixture::DefaultConstructionDerived, BinaryArchive> :
        public TableDataBase<::BinaryTableFixture::DefaultConstructionDerived, BinaryArchive>
    {
        Base<::BinaryTableFixture::Base> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<::BinaryTableFixture::DefaultConstructionDerived, BinaryArchive> : public
        TableScribe<::BinaryTableFixture::DefaultConstructionDerived, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        protected:
            void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
            {
                DoBasicConstruction(storage, archive);
            }
        };
    public:
        static const ClassNameResolver classNameResolver;
    };

    template<>
    struct TableData<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive> : public
        TableDataBase<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive>
    {
        Base<::BinaryTableFixture::Base> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive> : public
        TableScribe<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        protected:
            void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
        };
    public:
        static const ClassNameResolver classNameResolver;
    };

    template<>
    struct TableData<::BinaryTableFixture::ObjectScrivenBase, BinaryArchive> : public
        TableDataBase<::BinaryTableFixture::ObjectScrivenBase, BinaryArchive>
    {
        int baseValue;
    };

    template<>
    class Scribe<::BinaryTableFixture::ObjectScrivenBase, BinaryArchive> : public
        TableScribe<::BinaryTableFixture::ObjectScrivenBase, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        protected:
            void ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
            void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
            {
                DoBasicConstruction(storage, archive);
            }
        };
    };

    template<>
    struct TableData<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive> : public
        TableDataBase<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>
    {
        Base<::BinaryTableFixture::ObjectScrivenBase> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive> : public
        TableScribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();

            void Construct(ObjectT* storage, ArchiveT& archive);
        protected:
            void ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
            void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
            {

            }
        };
    public:
        static const ClassNameResolver classNameResolver;
    };

    template<>
    struct TableData<::BinaryTableFixture::TableConstructionBase, BinaryArchive> : public
        TableDataBase<::BinaryTableFixture::TableConstructionBase, BinaryArchive>
    {
        int baseValue;
    };

    template<>
    class Scribe<::BinaryTableFixture::TableConstructionBase, BinaryArchive> : public
        TableScribe<::BinaryTableFixture::TableConstructionBase, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        protected:
            void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
            {
                DoBasicConstruction(storage, archive);
            }
        };
    };

    template<>
    struct TableData<::BinaryTableFixture::TableConstructionDerived, BinaryArchive> : public
        TableDataBase<::BinaryTableFixture::TableConstructionDerived, BinaryArchive>
    {
        Base<::BinaryTableFixture::TableConstructionBase> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<::BinaryTableFixture::TableConstructionDerived, BinaryArchive> : public
        TableScribe<::BinaryTableFixture::TableConstructionDerived, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        protected:
            void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
            {
                DoBasicConstruction(storage, archive);
            }
        };
    public:
        static const ClassNameResolver classNameResolver;
    };
}

BinaryTableFixture::BinaryTableFixture()
{
    typeRegistrationContext.RegisterType<Base>();
    typeRegistrationContext.RegisterType<CustomConstructionDerived>();

    typeRegistrationContext.RegisterType<ObjectScrivenBase>();
    typeRegistrationContext.RegisterType<ObjectScrivenDerived>();

    typeRegistrationContext.RegisterType<DefaultConstructionDerived>();

    typeRegistrationContext.RegisterType<TableConstructionBase>();
    typeRegistrationContext.RegisterType<TableConstructionDerived>();
}

class BinaryTableFixture::Base
{
public:
    int baseValue;
public:
    Base(int baseValue) : baseValue(baseValue)
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

    TableConstructionDerived(const ::Inscription::TableData<TableConstructionDerived, ::Inscription::BinaryArchive>& data) :
        TableConstructionBase(data.base), derivedValue(data.derivedValue)
    {}
};

BOOST_FIXTURE_TEST_SUITE(BinaryTableTests, BinaryTableFixture)

BOOST_AUTO_TEST_CASE(DefaultConstruction_Serialization)
{
    DefaultConstructionDerived saved =
        dataGeneration.Generator<DefaultConstructionDerived>().RandomStack<int, std::string>();

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(saved);
    }

    DefaultConstructionDerived loaded;

    {
        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loaded);
    }

    BOOST_REQUIRE(loaded.baseValue == saved.baseValue);
    BOOST_REQUIRE(loaded.derivedValue == saved.derivedValue);
}

BOOST_AUTO_TEST_CASE(DefaultConstruction_Construction)
{
    Base* saved = dataGeneration.Generator<DefaultConstructionDerived>().RandomHeap<int, std::string>();
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

    BOOST_REQUIRE(loadedCasted != nullptr);
    BOOST_REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
    BOOST_REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);

    delete saved;
    delete loaded;
}

BOOST_AUTO_TEST_CASE(CustomConstruction_Serialization)
{
    CustomConstructionDerived saved =
        dataGeneration.Generator<CustomConstructionDerived>().RandomStack<int, std::string>();

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(saved);
    }

    CustomConstructionDerived loaded(0, "");

    {
        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loaded);
    }

    BOOST_REQUIRE(loaded.baseValue == saved.baseValue);
    BOOST_REQUIRE(loaded.derivedValue == saved.derivedValue);
}

BOOST_AUTO_TEST_CASE(CustomConstruction_Construction)
{
    Base* saved = dataGeneration.Generator<CustomConstructionDerived>().RandomHeap<int, std::string>();
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

    BOOST_REQUIRE(loadedCasted != nullptr);
    BOOST_REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
    BOOST_REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);

    delete saved;
    delete loaded;
}

BOOST_AUTO_TEST_CASE(TableConstruction_Serialization)
{
    TableConstructionDerived saved = dataGeneration.Generator<TableConstructionDerived>().RandomStack<int, std::string>();

    {
        auto outputArchive = CreateRegistered<OutputArchive>();
        outputArchive(saved);
    }

    TableConstructionDerived loaded(0, "");

    {
        auto inputArchive = CreateRegistered<InputArchive>();
        inputArchive(loaded);
    }

    BOOST_REQUIRE(loaded.baseValue == saved.baseValue);
    BOOST_REQUIRE(loaded.derivedValue == saved.derivedValue);
}

BOOST_AUTO_TEST_CASE(TableConstruction_Construction)
{
    TableConstructionBase* saved = dataGeneration.Generator<TableConstructionDerived>().RandomHeap<int, std::string>();
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

    BOOST_REQUIRE(loadedCasted != nullptr);
    BOOST_REQUIRE(loadedCasted->baseValue == savedCasted->baseValue);
    BOOST_REQUIRE(loadedCasted->derivedValue == savedCasted->derivedValue);

    delete saved;
    delete loaded;
}

BOOST_AUTO_TEST_CASE(ObjectScriven_Serialization)
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

BOOST_AUTO_TEST_CASE(ObjectScriven_Construction)
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

    Scribe<::BinaryTableFixture::DefaultConstructionDerived, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Base(data.base),
            DataEntry::Auto(&ObjectT::derivedValue, &DataT::derivedValue) });
    }

    const Scribe<::BinaryTableFixture::DefaultConstructionDerived, BinaryArchive>::ClassNameResolver
        Scribe<::BinaryTableFixture::DefaultConstructionDerived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("DefaultConstructionDerived");

    void Scribe<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive>::Table::ConstructImplementation(
        ObjectT* storage, ArchiveT& archive)
    {
        new (storage) ObjectT(
            data.base->baseValue,
            data.derivedValue
        );
    }

    Scribe<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Base(Type<::BinaryTableFixture::Base>{}),
            DataEntry::Auto(&ObjectT::derivedValue, &DataT::derivedValue) });
    }

    const Scribe<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive>::ClassNameResolver
        Scribe<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("CustomConstructionDerived");

    Scribe<::BinaryTableFixture::ObjectScrivenBase, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Auto(&ObjectT::baseValue, &DataT::baseValue) });
    }

    void Scribe<::BinaryTableFixture::ObjectScrivenBase, BinaryArchive>::Table::
        ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.baseObjectScrivenValue);
    }

    void Scribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>::Table::
        ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive(object.derivedObjectScrivenValue);
    }

    void Scribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>::Table::Construct(
        ObjectT* storage, ArchiveT& archive)
    {
        new (storage) ObjectT(
            data.base->baseValue,
            data.derivedValue
        );
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

    Scribe<::BinaryTableFixture::TableConstructionBase, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Auto(&ObjectT::baseValue, &DataT::baseValue) });
    }

    Scribe<::BinaryTableFixture::TableConstructionDerived, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Base(Type<::BinaryTableFixture::TableConstructionBase>{}),
            DataEntry::Auto(&ObjectT::derivedValue, &DataT::derivedValue) });
    }

    const Scribe<::BinaryTableFixture::TableConstructionDerived, BinaryArchive>::ClassNameResolver
        Scribe<::BinaryTableFixture::TableConstructionDerived, BinaryArchive>::classNameResolver =
        CreateSingleNameResolver("TableConstructionDerived");
}