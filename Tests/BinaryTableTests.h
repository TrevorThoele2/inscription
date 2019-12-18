#pragma once

#include "BinaryFixture.h"

#include <Inscription/TableScribe.h>

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

    class UsingEntriesDerived;
    class UsingEntryPointerDerived;

    struct NonDefault
    {
        std::string value;
        NonDefault(const std::string& value) : value(value)
        {}

        NonDefault(const NonDefault& arg) = default;

        NonDefault(const ::Inscription::BinaryTableData<NonDefault>& data);

        bool operator==(const NonDefault& arg) const
        {
            return value == arg.value;
        }
    };
};

namespace Inscription
{
    template<>
    struct TableData<::BinaryTableFixture::Base, BinaryArchive> :
        TableDataBase<::BinaryTableFixture::Base, BinaryArchive>
    {
        int baseValue;
    };

    template<>
    class Scribe<::BinaryTableFixture::Base, BinaryArchive> final :
        public TableScribe<::BinaryTableFixture::Base, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        public:
            Table();
        };
    };

    template<>
    struct TableData<::BinaryTableFixture::DefaultConstructionDerived, BinaryArchive> :
        TableDataBase<::BinaryTableFixture::DefaultConstructionDerived, BinaryArchive>
    {
        Base<::BinaryTableFixture::Base> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<::BinaryTableFixture::DefaultConstructionDerived, BinaryArchive> final :
        public TableScribe<::BinaryTableFixture::DefaultConstructionDerived, BinaryArchive>
    {
    public:
        class Table final : public TableBase
        {
        public:
            Table();
        };
    public:
        static Type OutputType(const ArchiveT& archive);
    };

    template<>
    struct TableData<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive> :
        TableDataBase<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive>
    {
        Base<::BinaryTableFixture::Base> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive> final :
        public TableScribe<::BinaryTableFixture::CustomConstructionDerived, BinaryArchive>
    {
    public:
        class Table final : public TableBase
        {
        public:
            Table();

            void Construct(ObjectT* storage, ArchiveT& archive);
        };
    public:
        static Type OutputType(const ArchiveT& archive);
    };

    template<>
    struct TableData<::BinaryTableFixture::ObjectScrivenBase, BinaryArchive> :
        TableDataBase<::BinaryTableFixture::ObjectScrivenBase, BinaryArchive>
    {
        int baseValue;
    };

    template<>
    class Scribe<::BinaryTableFixture::ObjectScrivenBase, BinaryArchive> final :
        public TableScribe<::BinaryTableFixture::ObjectScrivenBase, BinaryArchive>
    {
    public:
        class Table final : public TableBase
        {
        public:
            Table();
        protected:
            void ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        };
    };

    template<>
    struct TableData<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive> :
        TableDataBase<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>
    {
        Base<::BinaryTableFixture::ObjectScrivenBase> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive> final :
        public TableScribe<::BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>
    {
    public:
        class Table final : public TableBase
        {
        public:
            Table();

            void Construct(ObjectT* storage, ArchiveT& archive);
        protected:
            void ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        };
    public:
        static Type OutputType(const ArchiveT& archive);
    };

    template<>
    struct TableData<::BinaryTableFixture::TableConstructionBase, BinaryArchive> :
        TableDataBase<::BinaryTableFixture::TableConstructionBase, BinaryArchive>
    {
        int baseValue;
    };

    template<>
    class Scribe<::BinaryTableFixture::TableConstructionBase, BinaryArchive> final :
        public TableScribe<::BinaryTableFixture::TableConstructionBase, BinaryArchive>
    {
    public:
        class Table final : public TableBase
        {
        public:
            Table();
        };
    };

    template<>
    struct TableData<::BinaryTableFixture::TableConstructionDerived, BinaryArchive> :
        TableDataBase<::BinaryTableFixture::TableConstructionDerived, BinaryArchive>
    {
        Base<::BinaryTableFixture::TableConstructionBase> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<::BinaryTableFixture::TableConstructionDerived, BinaryArchive> final :
        public TableScribe<::BinaryTableFixture::TableConstructionDerived, BinaryArchive>
    {
    public:
        class Table final : public TableBase
        {
        public:
            Table();
        };
    public:
        static Type OutputType(const ArchiveT& archive);
    };

    template<>
    struct TableData<::BinaryTableFixture::UsingEntriesDerived, BinaryArchive> :
        TableDataBase<::BinaryTableFixture::UsingEntriesDerived, BinaryArchive>
    {
        Base<::BinaryTableFixture::Base> base;
        Entry<::BinaryTableFixture::NonDefault> derivedValue;
    };

    template<>
    class Scribe<::BinaryTableFixture::UsingEntriesDerived, BinaryArchive> final :
        public TableScribe<::BinaryTableFixture::UsingEntriesDerived, BinaryArchive>
    {
    public:
        class Table final : public TableBase
        {
        public:
            Table();
        };
    public:
        static Type OutputType(const ArchiveT& archive);
    };

    template<>
    struct TableData<::BinaryTableFixture::UsingEntryPointerDerived, BinaryArchive> :
        TableDataBase<::BinaryTableFixture::UsingEntryPointerDerived, BinaryArchive>
    {
        Base<::BinaryTableFixture::Base> base;
        ::BinaryTableFixture::NonDefault* derivedValue = nullptr;
    };

    template<>
    class Scribe<::BinaryTableFixture::UsingEntryPointerDerived, BinaryArchive> final :
        public TableScribe<::BinaryTableFixture::UsingEntryPointerDerived, BinaryArchive>
    {
    public:
        class Table final : public TableBase
        {
        public:
            Table();
        };
    public:
        static Type OutputType(const ArchiveT& archive);
    };

    template<>
    struct TableData<::BinaryTableFixture::NonDefault, BinaryArchive> :
        TableDataBase<::BinaryTableFixture::NonDefault, BinaryArchive>
    {
        std::string value;
    };

    template<>
    class Scribe<::BinaryTableFixture::NonDefault, BinaryArchive> final :
        public TableScribe<::BinaryTableFixture::NonDefault, BinaryArchive>
    {
    public:
        class Table final : public TableBase
        {
        public:
            Table();
        };
    };
}