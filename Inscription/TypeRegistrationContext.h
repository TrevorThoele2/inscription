#pragma once

#include <memory>
#include <vector>
#include <typeindex>

namespace Inscription
{
    template<class Format>
    class TypeRegistrationContext
    {
    public:
        TypeRegistrationContext();
        TypeRegistrationContext(const TypeRegistrationContext& arg);
        TypeRegistrationContext(TypeRegistrationContext&& arg) noexcept;

        TypeRegistrationContext& operator=(const TypeRegistrationContext& arg);
        TypeRegistrationContext& operator=(TypeRegistrationContext&& arg) noexcept;

        template<class T>
        void RegisterType();

        template<class T>
        [[nodiscard]] bool HasStored() const;

        void PushAll(Format& format);
    private:
        class EntryBase
        {
        public:
            virtual ~EntryBase() = 0;

            virtual EntryBase* Clone() const = 0;

            [[nodiscard]] virtual std::type_index Type() const = 0;
            virtual void PushTo(Format& format) const = 0;
        };

        template<class T>
        class Entry : public EntryBase
        {
        public:
            ~Entry() = default;

            Entry* Clone() const override;

            [[nodiscard]] std::type_index Type() const override;
            void PushTo(Format& format) const override;
        };

        using EntryBasePtr = std::unique_ptr<EntryBase>;
        using EntryList = std::vector<EntryBasePtr>;
        EntryList entryList;

        static void CopyEntryList(const EntryList& from, EntryList& to);
    };

    template<class Format>
    TypeRegistrationContext<Format>::TypeRegistrationContext()
    {}

    template<class Format>
    TypeRegistrationContext<Format>::TypeRegistrationContext(const TypeRegistrationContext& arg)
    {
        CopyEntryList(arg.entryList, entryList);
    }

    template<class Format>
    TypeRegistrationContext<Format>::TypeRegistrationContext(TypeRegistrationContext&& arg) noexcept :
        entryList(std::move(arg.entryList))
    {}

    template<class Format>
    TypeRegistrationContext<Format>& TypeRegistrationContext<Format>::operator=(const TypeRegistrationContext& arg)
    {
        CopyEntryList(arg.entryList, entryList);
        return *this;
    }

    template<class Format>
    TypeRegistrationContext<Format>& TypeRegistrationContext<Format>::operator=(TypeRegistrationContext&& arg) noexcept
    {
        entryList = std::move(arg.entryList);
        return *this;
    }

    template<class Format>
    template<class T>
    void TypeRegistrationContext<Format>::RegisterType()
    {
        entryList.push_back(EntryBasePtr(new Entry<T>()));
    }

    template<class Format>
    template<class T>
    bool TypeRegistrationContext<Format>::HasStored() const
    {
        auto checkType = std::type_index(typeid(T));
        for (auto& loop : entryList)
            if (loop->Type() == checkType)
                return true;

        return false;
    }

    template<class Format>
    void TypeRegistrationContext<Format>::PushAll(Format& format)
    {
        for (auto& loop : entryList)
            loop->PushTo(format);
    }

    template<class Format>
    TypeRegistrationContext<Format>::EntryBase::~EntryBase()
    {}

    template<class Format>
    template<class T>
    typename TypeRegistrationContext<Format>::template Entry<T>* TypeRegistrationContext<Format>::Entry<T>::Clone() const
    {
        return new Entry<T>();
    }

    template<class Format>
    template<class T>
    std::type_index TypeRegistrationContext<Format>::Entry<T>::Type() const
    {
        return std::type_index(typeid(T));
    }

    template<class Format>
    template<class T>
    void TypeRegistrationContext<Format>::Entry<T>::PushTo(Format& format) const
    {
        format.types.template RegisterType<T>();
    }

    template<class Format>
    void TypeRegistrationContext<Format>::CopyEntryList(const EntryList& from, EntryList& to)
    {
        to.clear();
        for (auto& loop : from)
            to.push_back(EntryBasePtr(loop->Clone()));
    }
}