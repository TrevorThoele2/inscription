#pragma once

#include <memory>
#include <vector>
#include <typeindex>

namespace Inscription
{
    template<class Archive>
    class TypeRegistrationContext
    {
    public:
        TypeRegistrationContext();
        TypeRegistrationContext(const TypeRegistrationContext& arg);
        TypeRegistrationContext(TypeRegistrationContext&& arg);

        TypeRegistrationContext& operator=(const TypeRegistrationContext& arg);
        TypeRegistrationContext& operator=(TypeRegistrationContext&& arg);

        template<class T>
        void RegisterType();

        template<class T>
        bool HasStored() const;

        void PushAll(Archive& archive);
    private:
        class EntryBase
        {
        public:
            virtual ~EntryBase() = 0;

            virtual EntryBase* Clone() const = 0;

            virtual std::type_index Type() const = 0;
            virtual void PushTo(Archive& archive) const = 0;
        };

        template<class T>
        class Entry : public EntryBase
        {
        public:
            Entry* Clone() const override;

            std::type_index Type() const override;
            void PushTo(Archive& archive) const override;
        };

        using EntryBasePtr = std::unique_ptr<EntryBase>;
        using EntryList = std::vector<EntryBasePtr>;
        EntryList entryList;

        static void CopyEntryList(const EntryList& from, EntryList& to);
    };

    template<class Archive>
    TypeRegistrationContext<Archive>::TypeRegistrationContext()
    {}

    template<class Archive>
    TypeRegistrationContext<Archive>::TypeRegistrationContext(const TypeRegistrationContext& arg)
    {
        CopyEntryList(arg.entryList, entryList);
    }

    template<class Archive>
    TypeRegistrationContext<Archive>::TypeRegistrationContext(TypeRegistrationContext&& arg) : entryList(std::move(arg.entryList))
    {}

    template<class Archive>
    TypeRegistrationContext<Archive>& TypeRegistrationContext<Archive>::operator=(const TypeRegistrationContext& arg)
    {
        CopyEntryList(arg.entryList, entryList);
        return *this;
    }

    template<class Archive>
    TypeRegistrationContext<Archive>& TypeRegistrationContext<Archive>::operator=(TypeRegistrationContext&& arg)
    {
        entryList = std::move(arg.entryList);
        return *this;
    }

    template<class Archive>
    template<class T>
    void TypeRegistrationContext<Archive>::RegisterType()
    {
        entryList.push_back(EntryBasePtr(new Entry<T>()));
    }

    template<class Archive>
    template<class T>
    bool TypeRegistrationContext<Archive>::HasStored() const
    {
        auto checkType = std::type_index(typeid(T));
        for (auto& loop : entryList)
            if (loop->Type() == checkType)
                return true;

        return false;
    }

    template<class Archive>
    void TypeRegistrationContext<Archive>::PushAll(Archive& archive)
    {
        for (auto& loop : entryList)
            loop->PushTo(archive);
    }

    template<class Archive>
    TypeRegistrationContext<Archive>::EntryBase::~EntryBase()
    {}

    template<class Archive>
    template<class T>
    typename TypeRegistrationContext<Archive>::template Entry<T>* TypeRegistrationContext<Archive>::Entry<T>::Clone() const
    {
        return new Entry<T>();
    }

    template<class Archive>
    template<class T>
    std::type_index TypeRegistrationContext<Archive>::Entry<T>::Type() const
    {
        return std::type_index(typeid(T));
    }

    template<class Archive>
    template<class T>
    void TypeRegistrationContext<Archive>::Entry<T>::PushTo(Archive& archive) const
    {
        archive.template RegisterType<T>();
    }

    template<class Archive>
    void TypeRegistrationContext<Archive>::CopyEntryList(const EntryList& from, EntryList& to)
    {
        to.clear();
        for (auto& loop : from)
            to.push_back(EntryBasePtr(loop->Clone()));
    }
}