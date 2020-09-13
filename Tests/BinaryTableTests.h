#pragma once

#include "BinaryFixture.h"

#include <Inscription/TableScribeCategory.h>

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
    struct TableData<BinaryTableFixture::Base, BinaryArchive> :
        TableDataBase<BinaryTableFixture::Base, BinaryArchive>
    {
        int baseValue;
    };

    template<>
    class Scribe<BinaryTableFixture::Base> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::Base, BinaryArchive>
        {
        public:
            Table();
        };
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::Base, BinaryArchive> final
    {
        using Category = TableScribeCategory<BinaryTableFixture::Base>;
    };

    template<>
    struct TableData<BinaryTableFixture::DefaultConstructionDerived, BinaryArchive> :
        TableDataBase<BinaryTableFixture::DefaultConstructionDerived, BinaryArchive>
    {
        Base<BinaryTableFixture::Base> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<BinaryTableFixture::DefaultConstructionDerived> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::DefaultConstructionDerived, BinaryArchive>
        {
        public:
            Table();
        };
    public:
        static Type OutputType(const BinaryArchive& archive);
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::DefaultConstructionDerived, BinaryArchive> final
    {
        using Category = TableScribeCategory<BinaryTableFixture::DefaultConstructionDerived>;
    };

    template<>
    struct TableData<BinaryTableFixture::CustomConstructionDerived, BinaryArchive> :
        TableDataBase<BinaryTableFixture::CustomConstructionDerived, BinaryArchive>
    {
        Base<BinaryTableFixture::Base> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<BinaryTableFixture::CustomConstructionDerived> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::CustomConstructionDerived, BinaryArchive>
        {
        public:
            Table();

            void Construct(ObjectT* storage, ArchiveT& archive);
        };
    public:
        static Type OutputType(const BinaryArchive& archive);
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::CustomConstructionDerived, BinaryArchive>
    {
        using Category = TableScribeCategory<BinaryTableFixture::CustomConstructionDerived>;
    };

    template<>
    struct TableData<BinaryTableFixture::ObjectScrivenBase, BinaryArchive> :
        TableDataBase<BinaryTableFixture::ObjectScrivenBase, BinaryArchive>
    {
        int baseValue;
    };

    template<>
    class Scribe<BinaryTableFixture::ObjectScrivenBase> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::ObjectScrivenBase, BinaryArchive>
        {
        public:
            Table();
        protected:
            void ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        };
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::ObjectScrivenBase, BinaryArchive>
    {
        using Category = TableScribeCategory<BinaryTableFixture::ObjectScrivenBase>;
    };

    template<>
    struct TableData<BinaryTableFixture::ObjectScrivenDerived, BinaryArchive> :
        TableDataBase<BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>
    {
        Base<BinaryTableFixture::ObjectScrivenBase> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<BinaryTableFixture::ObjectScrivenDerived> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>
        {
        public:
            Table();

            void Construct(ObjectT* storage, ArchiveT& archive);
        protected:
            void ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        };
    public:
        static Type OutputType(const BinaryArchive& archive);
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::ObjectScrivenDerived, BinaryArchive>
    {
        using Category = TableScribeCategory<BinaryTableFixture::ObjectScrivenDerived>;
    };

    template<>
    struct TableData<BinaryTableFixture::TableConstructionBase, BinaryArchive> :
        TableDataBase<BinaryTableFixture::TableConstructionBase, BinaryArchive>
    {
        int baseValue;
    };

    template<>
    class Scribe<BinaryTableFixture::TableConstructionBase> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::TableConstructionBase, BinaryArchive>
        {
        public:
            Table();
        };
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::TableConstructionBase, BinaryArchive>
    {
        using Category = TableScribeCategory<BinaryTableFixture::TableConstructionBase>;
    };

    template<>
    struct TableData<BinaryTableFixture::TableConstructionDerived, BinaryArchive> :
        TableDataBase<BinaryTableFixture::TableConstructionDerived, BinaryArchive>
    {
        Base<BinaryTableFixture::TableConstructionBase> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<BinaryTableFixture::TableConstructionDerived> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::TableConstructionDerived, BinaryArchive>
        {
        public:
            Table();
        };
    public:
        static Type OutputType(const BinaryArchive& archive);
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::TableConstructionDerived, BinaryArchive>
    {
        using Category = TableScribeCategory<BinaryTableFixture::TableConstructionDerived>;
    };

    template<>
    struct TableData<BinaryTableFixture::UsingEntriesDerived, BinaryArchive> :
        TableDataBase<BinaryTableFixture::UsingEntriesDerived, BinaryArchive>
    {
        Base<BinaryTableFixture::Base> base;
        Entry<BinaryTableFixture::NonDefault> derivedValue;
    };

    template<>
    class Scribe<BinaryTableFixture::UsingEntriesDerived> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::UsingEntriesDerived, BinaryArchive>
        {
        public:
            Table();
        };
    public:
        static Type OutputType(const BinaryArchive& archive);
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::UsingEntriesDerived, BinaryArchive>
    {
        using Category = TableScribeCategory<BinaryTableFixture::UsingEntriesDerived>;
    };

    template<>
    struct TableData<BinaryTableFixture::UsingEntryPointerDerived, BinaryArchive> :
        TableDataBase<BinaryTableFixture::UsingEntryPointerDerived, BinaryArchive>
    {
        Base<BinaryTableFixture::Base> base;
        BinaryTableFixture::NonDefault* derivedValue = nullptr;
    };

    template<>
    class Scribe<BinaryTableFixture::UsingEntryPointerDerived> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::UsingEntryPointerDerived, BinaryArchive>
        {
        public:
            Table();
        };
    public:
        static Type OutputType(const BinaryArchive& archive);
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::UsingEntryPointerDerived, BinaryArchive>
    {
        using Category = TableScribeCategory<BinaryTableFixture::UsingEntryPointerDerived>;
    };

    template<>
    struct TableData<BinaryTableFixture::NonDefault, BinaryArchive> :
        TableDataBase<BinaryTableFixture::NonDefault, BinaryArchive>
    {
        std::string value;
    };

    template<>
    class Scribe<BinaryTableFixture::NonDefault> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::NonDefault, BinaryArchive>
        {
        public:
            Table();
        };
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::NonDefault, BinaryArchive>
    {
        using Category = TableScribeCategory<BinaryTableFixture::NonDefault>;
    };
}