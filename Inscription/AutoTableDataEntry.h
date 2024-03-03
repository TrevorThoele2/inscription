#pragma once

#include <typeindex>

#include "Type.h"

#include "Optional.h"

namespace Inscription
{
    template<class Data, class Object, class Archive>
    class AutoTableDataEntry
    {
    public:
        using ObjectT = Object;
        using ArchiveT = Archive;

        using DataT = Data;
    public:
        template<class T>
        static AutoTableDataEntry Auto(T ObjectT::*objectMember, T DataT::*dataMember);
        template<class BaseT>
        static AutoTableDataEntry Base(Type<BaseT>);

        AutoTableDataEntry(const AutoTableDataEntry& arg);
        AutoTableDataEntry(AutoTableDataEntry&& arg);
        
        AutoTableDataEntry& operator=(const AutoTableDataEntry& arg);
        AutoTableDataEntry& operator=(AutoTableDataEntry&& arg);

        void Scriven(DataT& data, ArchiveT& archive);
        void ObjectScriven(DataT& data, ObjectT& object, ArchiveT& archive);

        void PullFromObject(DataT& data, ObjectT& object);
        void PushToObject(DataT& data, ObjectT& object);
    public:
        template<class BaseT>
        typename Scribe<BaseT, ArchiveT>::Table::DataT* AsBase() const;

        bool IsBase() const;
        template<class BaseT>
        bool IsBase() const;
    private:
        template<class T>
        AutoTableDataEntry(T ObjectT::*objectMember, T DataT::*dataMember);
        template<class BaseT>
        AutoTableDataEntry(Type<BaseT>);
    private:
        class Implementation
        {
        public:
            virtual ~Implementation() = 0;

            virtual Implementation* Clone() const = 0;

            virtual void Scriven(DataT& data, ArchiveT& archive) = 0;
            virtual void ObjectScriven(DataT& data, ObjectT& object, ArchiveT& archive) = 0;

            virtual void PullFromObject(DataT& data, ObjectT& object) = 0;
            virtual void PushToObject(DataT& data, ObjectT& object) = 0;
        };

        template<class T>
        class AutoImplementation : public Implementation
        {
        public:
            T ObjectT::*objectMember;
            T DataT::*dataMember;
        public:
            AutoImplementation(T ObjectT::*objectMember, T DataT::*dataMember);

            AutoImplementation* Clone() const override;

            void Scriven(DataT& data, ArchiveT& archive) override;
            void ObjectScriven(DataT& data, ObjectT& object, ArchiveT& archive) override;

            void PullFromObject(DataT& data, ObjectT& object) override;
            void PushToObject(DataT& data, ObjectT& object) override;
        };

        template<class BaseT>
        class BaseImplementation : public Implementation
        {
        public:
            using BaseTable = typename Scribe<BaseT, ArchiveT>::Table;
            BaseTable baseTable;

            using BaseData = typename BaseTable::DataT;
            BaseData baseData;
        public:
            BaseImplementation* Clone() const override;

            void Scriven(DataT& data, ArchiveT& archive) override;
            void ObjectScriven(DataT& data, ObjectT& object, ArchiveT& archive) override;

            void PullFromObject(DataT& data, ObjectT& object) override;
            void PushToObject(DataT& data, ObjectT& object) override;
        };

        using ImplementationPtr = std::unique_ptr<Implementation>;
        ImplementationPtr implementation;
    private:
        Optional<std::type_index> baseType;
    };

    template<class Data, class Object, class Archive>
    template<class T>
    AutoTableDataEntry<Data, Object, Archive> AutoTableDataEntry<Data, Object, Archive>::Auto(
        T ObjectT::*objectMember, T DataT::*dataMember)
    {
        return AutoTableDataEntry(objectMember, dataMember);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    AutoTableDataEntry<Data, Object, Archive> AutoTableDataEntry<Data, Object, Archive>::Base(
        Type<BaseT> type)
    {
        return AutoTableDataEntry(type);
    }

    template<class Data, class Object, class Archive>
    AutoTableDataEntry<Data, Object, Archive>::AutoTableDataEntry(const AutoTableDataEntry& arg) :
        implementation(arg.implementation->Clone()), baseType(arg.baseType)
    {}

    template<class Data, class Object, class Archive>
    AutoTableDataEntry<Data, Object, Archive>::AutoTableDataEntry(AutoTableDataEntry&& arg) :
        implementation(std::move(arg.implementation)), baseType(std::move(arg.baseType))
    {}

    template<class Data, class Object, class Archive>
    AutoTableDataEntry<Data, Object, Archive>& AutoTableDataEntry<Data, Object, Archive>::operator=(
        const AutoTableDataEntry& arg)
    {
        implementation.reset(arg.implementation->Clone());
        baseType = arg.baseType;
        return *this;
    }

    template<class Data, class Object, class Archive>
    AutoTableDataEntry<Data, Object, Archive>& AutoTableDataEntry<Data, Object, Archive>::operator=(
        AutoTableDataEntry&& arg)
    {
        implementation = std::move(arg.implementation);
        baseType = std::move(arg.baseType);
        return *this;
    }

    template<class Data, class Object, class Archive>
    void AutoTableDataEntry<Data, Object, Archive>::Scriven(DataT& data, ArchiveT& archive)
    {
        implementation->Scriven(data, archive);
    }

    template<class Data, class Object, class Archive>
    void AutoTableDataEntry<Data, Object, Archive>::ObjectScriven(DataT& data, ObjectT& object, ArchiveT& archive)
    {
        implementation->ObjectScriven(data, object, archive);
    }

    template<class Data, class Object, class Archive>
    void AutoTableDataEntry<Data, Object, Archive>::PullFromObject(DataT& data, ObjectT& object)
    {
        implementation->PullFromObject(data, object);
    }

    template<class Data, class Object, class Archive>
    void AutoTableDataEntry<Data, Object, Archive>::PushToObject(DataT& data, ObjectT& object)
    {
        implementation->PushToObject(data, object);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    typename Scribe<BaseT, Archive>::Table::DataT* AutoTableDataEntry<Data, Object, Archive>::AsBase() const
    {
        if (!IsBase<BaseT>())
            return nullptr;

        return &static_cast<BaseImplementation<BaseT>*>(implementation.get())->baseData;
    }

    template<class Data, class Object, class Archive>
    bool AutoTableDataEntry<Data, Object, Archive>::IsBase() const
    {
        return baseType.IsValid();
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    bool AutoTableDataEntry<Data, Object, Archive>::IsBase() const
    {
        return baseType == typeid(BaseT);
    }

    template<class Data, class Object, class Archive>
    template<class T>
    AutoTableDataEntry<Data, Object, Archive>::AutoTableDataEntry(T ObjectT::*objectMember, T DataT::*dataMember) :
        implementation(new AutoImplementation<T>(objectMember, dataMember))
    {}

    template<class Data, class Object, class Archive>
    template<class BaseT>
    AutoTableDataEntry<Data, Object, Archive>::AutoTableDataEntry(Type<BaseT>) :
        implementation(new BaseImplementation<BaseT>()), baseType(std::type_index(typeid(BaseT)))
    {}

    template<class Data, class Object, class Archive>
    AutoTableDataEntry<Data, Object, Archive>::Implementation::~Implementation()
    {}

    template<class Data, class Object, class Archive>
    template<class T>
    AutoTableDataEntry<Data, Object, Archive>::AutoImplementation<T>::AutoImplementation(
        T ObjectT::*objectMember, T DataT::*dataMember) :

        objectMember(objectMember), dataMember(dataMember)
    {}

    template<class Data, class Object, class Archive>
    template<class T>
    AutoTableDataEntry<Data, Object, Archive>::AutoImplementation<T>*
        AutoTableDataEntry<Data, Object, Archive>::AutoImplementation<T>::Clone() const
    {
        return new AutoImplementation(*this);
    }

    template<class Data, class Object, class Archive>
    template<class T>
    void AutoTableDataEntry<Data, Object, Archive>::AutoImplementation<T>::Scriven(
        DataT& data, ArchiveT& archive)
    {
        archive(data.*dataMember);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataEntry<Data, Object, Archive>::AutoImplementation<BaseT>::ObjectScriven(
        DataT& data, ObjectT& object, ArchiveT& archive)
    {}

    template<class Data, class Object, class Archive>
    template<class T>
    void AutoTableDataEntry<Data, Object, Archive>::AutoImplementation<T>::PullFromObject(
        DataT& data, ObjectT& object)
    {
        (data.*dataMember) = (object.*objectMember);
    }

    template<class Data, class Object, class Archive>
    template<class T>
    void AutoTableDataEntry<Data, Object, Archive>::AutoImplementation<T>::PushToObject(
        DataT& data, ObjectT& object)
    {
        (object.*objectMember) = (data.*dataMember);
    }

    template<class Data, class Object, class Archive>
    template<class T>
    AutoTableDataEntry<Data, Object, Archive>::BaseImplementation<T>*
        AutoTableDataEntry<Data, Object, Archive>::BaseImplementation<T>::Clone() const
    {
        return new BaseImplementation(*this);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataEntry<Data, Object, Archive>::BaseImplementation<BaseT>::Scriven(
        DataT& data, ArchiveT& archive)
    {
        baseTable.Scriven(baseData, archive);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataEntry<Data, Object, Archive>::BaseImplementation<BaseT>::ObjectScriven(
        DataT& data, ObjectT& object, ArchiveT& archive)
    {
        baseTable.ObjectScriven(baseData, object, archive);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataEntry<Data, Object, Archive>::BaseImplementation<BaseT>::PullFromObject(
        DataT& data, ObjectT& object)
    {
        baseTable.PullFromObject(baseData, object);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataEntry<Data, Object, Archive>::BaseImplementation<BaseT>::PushToObject(
        DataT& data, ObjectT& object)
    {
        baseTable.PushToObject(baseData, object);
    }
}