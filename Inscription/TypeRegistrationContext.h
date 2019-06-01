#pragma once

#include <memory>
#include <vector>
#include <typeindex>

namespace Inscription
{
    template<class ScribeT>
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

        void PushAll(ScribeT& scribe);
    private:
        class EntryBase
        {
        public:
            virtual ~EntryBase() = 0;

            virtual EntryBase* Clone() const = 0;

            virtual std::type_index Type() const = 0;
            virtual void PushTo(ScribeT& scribe) const = 0;
        };

        template<class T>
        class Entry : public EntryBase
        {
        public:
            Entry* Clone() const override;

            std::type_index Type() const override;
            void PushTo(ScribeT& scribe) const override;
        };

        typedef std::unique_ptr<EntryBase> EntryBasePtr;
        typedef std::vector<EntryBasePtr> EntryList;
        EntryList entryList;

        static void CopyEntryList(const EntryList& from, EntryList& to);
    };

    template<class ScribeT>
    TypeRegistrationContext<ScribeT>::TypeRegistrationContext()
    {}

    template<class ScribeT>
    TypeRegistrationContext<ScribeT>::TypeRegistrationContext(const TypeRegistrationContext& arg)
    {
        CopyEntryList(arg.entryList, entryList);
    }

    template<class ScribeT>
    TypeRegistrationContext<ScribeT>::TypeRegistrationContext(TypeRegistrationContext&& arg) : entryList(std::move(arg.entryList))
    {}

    template<class ScribeT>
    TypeRegistrationContext<ScribeT>& TypeRegistrationContext<ScribeT>::operator=(const TypeRegistrationContext& arg)
    {
        CopyEntryList(arg.entryList, entryList);
        return *this;
    }

    template<class ScribeT>
    TypeRegistrationContext<ScribeT>& TypeRegistrationContext<ScribeT>::operator=(TypeRegistrationContext&& arg)
    {
        entryList = std::move(arg.entryList);
        return *this;
    }

    template<class ScribeT>
    template<class T>
    void TypeRegistrationContext<ScribeT>::RegisterType()
    {
        entryList.push_back(EntryBasePtr(new Entry<T>()));
    }

    template<class ScribeT>
    template<class T>
    bool TypeRegistrationContext<ScribeT>::HasStored() const
    {
        auto checkType = std::type_index(typeid(T));
        for (auto& loop : entryList)
            if (loop->Type() == checkType)
                return true;

        return false;
    }

    template<class ScribeT>
    void TypeRegistrationContext<ScribeT>::PushAll(ScribeT& scribe)
    {
        for (auto& loop : entryList)
            loop->PushTo(scribe);
    }

    template<class ScribeT>
    TypeRegistrationContext<ScribeT>::EntryBase::~EntryBase()
    {}

    template<class ScribeT>
    template<class T>
    typename TypeRegistrationContext<ScribeT>::template Entry<T>* TypeRegistrationContext<ScribeT>::Entry<T>::Clone() const
    {
        return new Entry<T>();
    }

    template<class ScribeT>
    template<class T>
    std::type_index TypeRegistrationContext<ScribeT>::Entry<T>::Type() const
    {
        return std::type_index(typeid(T));
    }

    template<class ScribeT>
    template<class T>
    void TypeRegistrationContext<ScribeT>::Entry<T>::PushTo(ScribeT& scribe) const
    {
        scribe.template RegisterType<T>();
    }

    template<class ScribeT>
    void TypeRegistrationContext<ScribeT>::CopyEntryList(const EntryList& from, EntryList& to)
    {
        to.clear();
        for (auto& loop : from)
            to.push_back(EntryBasePtr(loop->Clone()));
    }
}