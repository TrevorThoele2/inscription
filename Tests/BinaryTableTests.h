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

        NonDefault(const Inscription::BinaryTableData<NonDefault>& data);

        bool operator==(const NonDefault& arg) const
        {
            return value == arg.value;
        }
    };
};

namespace Inscription
{
    template<>
    struct TableData<BinaryTableFixture::Base, Archive::Binary> :
        TableDataBase<BinaryTableFixture::Base, Archive::Binary>
    {
        int baseValue;
    };

    template<>
    class Scribe<BinaryTableFixture::Base> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::Base, Archive::Binary>
        {
        public:
            Table();
        };
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::Base, Archive::Binary> final
    {
        using Category = TableScribeCategory<BinaryTableFixture::Base>;
    };

    template<>
    struct TableData<BinaryTableFixture::DefaultConstructionDerived, Archive::Binary> :
        TableDataBase<BinaryTableFixture::DefaultConstructionDerived, Archive::Binary>
    {
        Base<BinaryTableFixture::Base> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<BinaryTableFixture::DefaultConstructionDerived> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::DefaultConstructionDerived, Archive::Binary>
        {
        public:
            Table();
        };
    public:
        static Type OutputType(const Archive::Binary& archive);
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::DefaultConstructionDerived, Archive::Binary> final
    {
        using Category = TableScribeCategory<BinaryTableFixture::DefaultConstructionDerived>;
    };

    template<>
    struct TableData<BinaryTableFixture::CustomConstructionDerived, Archive::Binary> :
        TableDataBase<BinaryTableFixture::CustomConstructionDerived, Archive::Binary>
    {
        Base<BinaryTableFixture::Base> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<BinaryTableFixture::CustomConstructionDerived> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::CustomConstructionDerived, Archive::Binary>
        {
        public:
            Table();

            void Construct(ObjectT* storage, ArchiveT& archive);
        };
    public:
        static Type OutputType(const Archive::Binary& archive);
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::CustomConstructionDerived, Archive::Binary>
    {
        using Category = TableScribeCategory<BinaryTableFixture::CustomConstructionDerived>;
    };

    template<>
    struct TableData<BinaryTableFixture::ObjectScrivenBase, Archive::Binary> :
        TableDataBase<BinaryTableFixture::ObjectScrivenBase, Archive::Binary>
    {
        int baseValue;
    };

    template<>
    class Scribe<BinaryTableFixture::ObjectScrivenBase> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::ObjectScrivenBase, Archive::Binary>
        {
        public:
            Table();
        protected:
            void ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        };
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::ObjectScrivenBase, Archive::Binary>
    {
        using Category = TableScribeCategory<BinaryTableFixture::ObjectScrivenBase>;
    };

    template<>
    struct TableData<BinaryTableFixture::ObjectScrivenDerived, Archive::Binary> :
        TableDataBase<BinaryTableFixture::ObjectScrivenDerived, Archive::Binary>
    {
        Base<BinaryTableFixture::ObjectScrivenBase> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<BinaryTableFixture::ObjectScrivenDerived> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::ObjectScrivenDerived, Archive::Binary>
        {
        public:
            Table();

            void Construct(ObjectT* storage, ArchiveT& archive);
        protected:
            void ObjectScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        };
    public:
        static Type OutputType(const Archive::Binary& archive);
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::ObjectScrivenDerived, Archive::Binary>
    {
        using Category = TableScribeCategory<BinaryTableFixture::ObjectScrivenDerived>;
    };

    template<>
    struct TableData<BinaryTableFixture::TableConstructionBase, Archive::Binary> :
        TableDataBase<BinaryTableFixture::TableConstructionBase, Archive::Binary>
    {
        int baseValue;
    };

    template<>
    class Scribe<BinaryTableFixture::TableConstructionBase> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::TableConstructionBase, Archive::Binary>
        {
        public:
            Table();
        };
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::TableConstructionBase, Archive::Binary>
    {
        using Category = TableScribeCategory<BinaryTableFixture::TableConstructionBase>;
    };

    template<>
    struct TableData<BinaryTableFixture::TableConstructionDerived, Archive::Binary> :
        TableDataBase<BinaryTableFixture::TableConstructionDerived, Archive::Binary>
    {
        Base<BinaryTableFixture::TableConstructionBase> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<BinaryTableFixture::TableConstructionDerived> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::TableConstructionDerived, Archive::Binary>
        {
        public:
            Table();
        };
    public:
        static Type OutputType(const Archive::Binary& archive);
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::TableConstructionDerived, Archive::Binary>
    {
        using Category = TableScribeCategory<BinaryTableFixture::TableConstructionDerived>;
    };

    template<>
    struct TableData<BinaryTableFixture::UsingEntriesDerived, Archive::Binary> :
        TableDataBase<BinaryTableFixture::UsingEntriesDerived, Archive::Binary>
    {
        Base<BinaryTableFixture::Base> base;
        Entry<BinaryTableFixture::NonDefault> derivedValue;
    };

    template<>
    class Scribe<BinaryTableFixture::UsingEntriesDerived> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::UsingEntriesDerived, Archive::Binary>
        {
        public:
            Table();
        };
    public:
        static Type OutputType(const Archive::Binary& archive);
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::UsingEntriesDerived, Archive::Binary>
    {
        using Category = TableScribeCategory<BinaryTableFixture::UsingEntriesDerived>;
    };

    template<>
    struct TableData<BinaryTableFixture::UsingEntryPointerDerived, Archive::Binary> :
        TableDataBase<BinaryTableFixture::UsingEntryPointerDerived, Archive::Binary>
    {
        Base<BinaryTableFixture::Base> base;
        BinaryTableFixture::NonDefault* derivedValue = nullptr;
    };

    template<>
    class Scribe<BinaryTableFixture::UsingEntryPointerDerived> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::UsingEntryPointerDerived, Archive::Binary>
        {
        public:
            Table();
        };
    public:
        static Type OutputType(const Archive::Binary& archive);
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::UsingEntryPointerDerived, Archive::Binary>
    {
        using Category = TableScribeCategory<BinaryTableFixture::UsingEntryPointerDerived>;
    };

    template<>
    struct TableData<BinaryTableFixture::NonDefault, Archive::Binary> :
        TableDataBase<BinaryTableFixture::NonDefault, Archive::Binary>
    {
        std::string value;
    };

    template<>
    class Scribe<BinaryTableFixture::NonDefault> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::NonDefault, Archive::Binary>
        {
        public:
            Table();
        };
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::NonDefault, Archive::Binary>
    {
        using Category = TableScribeCategory<BinaryTableFixture::NonDefault>;
    };
}