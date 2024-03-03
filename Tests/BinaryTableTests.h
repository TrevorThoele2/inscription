#pragma once

#include "GeneralFixture.h"

#include <Inscription/TableScribeCategory.h>

class BinaryTableFixture : public GeneralFixture
{
public:
    Inscription::TypeRegistrationContext<Inscription::Format::Binary> typeRegistrationContext;

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
    struct TableData<BinaryTableFixture::Base, Format::Binary> :
        TableDataBase<BinaryTableFixture::Base, Format::Binary>
    {
        int baseValue;
    };

    template<>
    class Scribe<BinaryTableFixture::Base> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::Base, Format::Binary>
        {
        public:
            Table();
        };
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::Base, Format::Binary> final
    {
        using Category = TableScribeCategory<BinaryTableFixture::Base>;
    };

    template<>
    struct TableData<BinaryTableFixture::DefaultConstructionDerived, Format::Binary> :
        TableDataBase<BinaryTableFixture::DefaultConstructionDerived, Format::Binary>
    {
        Base<BinaryTableFixture::Base> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<BinaryTableFixture::DefaultConstructionDerived> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::DefaultConstructionDerived, Format::Binary>
        {
        public:
            Table();
        };
    public:
        static Type OutputType(const Format::Binary& format);
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::DefaultConstructionDerived, Format::Binary> final
    {
        using Category = TableScribeCategory<BinaryTableFixture::DefaultConstructionDerived>;
    };

    template<>
    struct TableData<BinaryTableFixture::CustomConstructionDerived, Format::Binary> :
        TableDataBase<BinaryTableFixture::CustomConstructionDerived, Format::Binary>
    {
        Base<BinaryTableFixture::Base> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<BinaryTableFixture::CustomConstructionDerived> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::CustomConstructionDerived, Format::Binary>
        {
        public:
            Table();

            void Construct(ObjectT* storage, FormatT& format);
        };
    public:
        static Type OutputType(const Format::Binary& format);
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::CustomConstructionDerived, Format::Binary>
    {
        using Category = TableScribeCategory<BinaryTableFixture::CustomConstructionDerived>;
    };

    template<>
    struct TableData<BinaryTableFixture::ObjectScrivenBase, Format::Binary> :
        TableDataBase<BinaryTableFixture::ObjectScrivenBase, Format::Binary>
    {
        int baseValue;
    };

    template<>
    class Scribe<BinaryTableFixture::ObjectScrivenBase> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::ObjectScrivenBase, Format::Binary>
        {
        public:
            Table();
        protected:
            void ObjectScrivenImplementation(ObjectT& object, FormatT& format) override;
        };
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::ObjectScrivenBase, Format::Binary>
    {
        using Category = TableScribeCategory<BinaryTableFixture::ObjectScrivenBase>;
    };

    template<>
    struct TableData<BinaryTableFixture::ObjectScrivenDerived, Format::Binary> :
        TableDataBase<BinaryTableFixture::ObjectScrivenDerived, Format::Binary>
    {
        Base<BinaryTableFixture::ObjectScrivenBase> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<BinaryTableFixture::ObjectScrivenDerived> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::ObjectScrivenDerived, Format::Binary>
        {
        public:
            Table();

            void Construct(ObjectT* storage, FormatT& format);
        protected:
            void ObjectScrivenImplementation(ObjectT& object, FormatT& format) override;
        };
    public:
        static Type OutputType(const Format::Binary& format);
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::ObjectScrivenDerived, Format::Binary>
    {
        using Category = TableScribeCategory<BinaryTableFixture::ObjectScrivenDerived>;
    };

    template<>
    struct TableData<BinaryTableFixture::TableConstructionBase, Format::Binary> :
        TableDataBase<BinaryTableFixture::TableConstructionBase, Format::Binary>
    {
        int baseValue;
    };

    template<>
    class Scribe<BinaryTableFixture::TableConstructionBase> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::TableConstructionBase, Format::Binary>
        {
        public:
            Table();
        };
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::TableConstructionBase, Format::Binary>
    {
        using Category = TableScribeCategory<BinaryTableFixture::TableConstructionBase>;
    };

    template<>
    struct TableData<BinaryTableFixture::TableConstructionDerived, Format::Binary> :
        TableDataBase<BinaryTableFixture::TableConstructionDerived, Format::Binary>
    {
        Base<BinaryTableFixture::TableConstructionBase> base;
        std::string derivedValue;
    };

    template<>
    class Scribe<BinaryTableFixture::TableConstructionDerived> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::TableConstructionDerived, Format::Binary>
        {
        public:
            Table();
        };
    public:
        static Type OutputType(const Format::Binary& format);
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::TableConstructionDerived, Format::Binary>
    {
        using Category = TableScribeCategory<BinaryTableFixture::TableConstructionDerived>;
    };

    template<>
    struct TableData<BinaryTableFixture::UsingEntriesDerived, Format::Binary> :
        TableDataBase<BinaryTableFixture::UsingEntriesDerived, Format::Binary>
    {
        Base<BinaryTableFixture::Base> base;
        Entry<BinaryTableFixture::NonDefault> derivedValue;
    };

    template<>
    class Scribe<BinaryTableFixture::UsingEntriesDerived> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::UsingEntriesDerived, Format::Binary>
        {
        public:
            Table();
        };
    public:
        static Type OutputType(const Format::Binary& format);
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::UsingEntriesDerived, Format::Binary>
    {
        using Category = TableScribeCategory<BinaryTableFixture::UsingEntriesDerived>;
    };

    template<>
    struct TableData<BinaryTableFixture::UsingEntryPointerDerived, Format::Binary> :
        TableDataBase<BinaryTableFixture::UsingEntryPointerDerived, Format::Binary>
    {
        Base<BinaryTableFixture::Base> base;
        BinaryTableFixture::NonDefault* derivedValue = nullptr;
    };

    template<>
    class Scribe<BinaryTableFixture::UsingEntryPointerDerived> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::UsingEntryPointerDerived, Format::Binary>
        {
        public:
            Table();
        };
    public:
        static Type OutputType(const Format::Binary& format);
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::UsingEntryPointerDerived, Format::Binary>
    {
        using Category = TableScribeCategory<BinaryTableFixture::UsingEntryPointerDerived>;
    };

    template<>
    struct TableData<BinaryTableFixture::NonDefault, Format::Binary> :
        TableDataBase<BinaryTableFixture::NonDefault, Format::Binary>
    {
        std::string value;
    };

    template<>
    class Scribe<BinaryTableFixture::NonDefault> final
    {
    public:
        class Table final : public TableBase<BinaryTableFixture::NonDefault, Format::Binary>
        {
        public:
            Table();
        };
    };

    template<>
    struct ScribeTraits<BinaryTableFixture::NonDefault, Format::Binary>
    {
        using Category = TableScribeCategory<BinaryTableFixture::NonDefault>;
    };
}