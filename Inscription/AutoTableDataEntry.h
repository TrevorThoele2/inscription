#pragma once

#include <typeindex>

#include "BaseTableDataEntry.h"
#include "Type.h"

namespace Inscription
{
    template<class Table, class Object, class Archive>
    class AutoTableDataEntry
    {
    public:
        using TableT = Table;
        using ObjectT = Object;
        using ArchiveT = Archive;

        using DataT = typename TableT::DataT;
    public:
        template<class ObjectMemberT, class DataMemberT>
        static AutoTableDataEntry Auto(ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember);
        template<class BaseT>
        static AutoTableDataEntry Base(Type<BaseT>);
        template<class BaseT>
        static AutoTableDataEntry Base(BaseTableDataEntry<BaseT, ObjectT, ArchiveT>& entry);

        AutoTableDataEntry(const AutoTableDataEntry& arg);
        AutoTableDataEntry(AutoTableDataEntry&& arg);
        
        AutoTableDataEntry& operator=(const AutoTableDataEntry& arg);
        AutoTableDataEntry& operator=(AutoTableDataEntry&& arg);

        void Scriven(TableT& table, ArchiveT& archive);
        void ObjectScriven(TableT& table, ObjectT& object, ArchiveT& archive);

        void PullFromObject(TableT& table, ObjectT& object);
        void PushToObject(TableT& table, ObjectT& object);
    private:
        template<class ObjectMemberT, class DataMemberT>
        AutoTableDataEntry(ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember);
        template<class BaseT>
        AutoTableDataEntry(Type<BaseT>);
        template<class BaseT>
        AutoTableDataEntry(BaseTableDataEntry<BaseT, ObjectT, ArchiveT>& entry);
    private:
        class Implementation
        {
        public:
            virtual ~Implementation() = 0;

            virtual Implementation* Clone() const = 0;

            virtual void Scriven(TableT& table, ArchiveT& archive) = 0;
            virtual void ObjectScriven(TableT& table, ObjectT& object, ArchiveT& archive) = 0;

            virtual void PullFromObject(TableT& table, ObjectT& object) = 0;
            virtual void PushToObject(TableT& table, ObjectT& object) = 0;
        };

        template<class ObjectMemberT, class DataMemberT>
        class AutoImplementation : public Implementation
        {
        public:
            ObjectMemberT ObjectT::*objectMember;
            DataMemberT DataT::*dataMember;
        public:
            AutoImplementation(ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember);

            AutoImplementation* Clone() const override;

            void Scriven(TableT& table, ArchiveT& archive) override;
            void ObjectScriven(TableT& table, ObjectT& object, ArchiveT& archive) override;

            void PullFromObject(TableT& table, ObjectT& object) override;
            void PushToObject(TableT& table, ObjectT& object) override;
        };

        template<class BaseT>
        class BaseImplementation : public Implementation
        {
        public:
            using BaseEntry = BaseTableDataEntry<BaseT, ObjectT, ArchiveT>;
            BaseEntry baseEntry;
        public:
            BaseImplementation* Clone() const override;

            void Scriven(TableT& table, ArchiveT& archive) override;
            void ObjectScriven(TableT& table, ObjectT& object, ArchiveT& archive) override;

            void PullFromObject(TableT& table, ObjectT& object) override;
            void PushToObject(TableT& table, ObjectT& object) override;
        };

        template<class BaseT>
        class BaseReferenceImplementation : public Implementation
        {
        public:
            using BaseEntry = BaseTableDataEntry<BaseT, ObjectT, ArchiveT>;
            BaseEntry* baseEntry;
        public:
            BaseReferenceImplementation(BaseEntry& baseEntry);

            BaseReferenceImplementation* Clone() const override;

            void Scriven(TableT& table, ArchiveT& archive) override;
            void ObjectScriven(TableT& table, ObjectT& object, ArchiveT& archive) override;

            void PullFromObject(TableT& table, ObjectT& object) override;
            void PushToObject(TableT& table, ObjectT& object) override;
        };

        using ImplementationPtr = std::unique_ptr<Implementation>;
        ImplementationPtr implementation;
    };

    template<class Data, class Object, class Archive>
    template<class ObjectMemberT, class DataMemberT>
    AutoTableDataEntry<Data, Object, Archive> AutoTableDataEntry<Data, Object, Archive>::Auto(
        ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember)
    {
        static_assert(
            std::is_convertible_v<ObjectMemberT, DataMemberT> &&
            std::is_convertible_v<DataMemberT, ObjectMemberT>,
            "ObjectMemberT and DataMemberT must be convertible to each other.");

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
    template<class BaseT>
    AutoTableDataEntry<Data, Object, Archive> AutoTableDataEntry<Data, Object, Archive>::Base(
        BaseTableDataEntry<BaseT, ObjectT, ArchiveT>& entry)
    {
        return AutoTableDataEntry(entry);
    }

    template<class Data, class Object, class Archive>
    AutoTableDataEntry<Data, Object, Archive>::AutoTableDataEntry(const AutoTableDataEntry& arg) :
        implementation(arg.implementation->Clone())
    {}

    template<class Data, class Object, class Archive>
    AutoTableDataEntry<Data, Object, Archive>::AutoTableDataEntry(AutoTableDataEntry&& arg) :
        implementation(std::move(arg.implementation))
    {}

    template<class Data, class Object, class Archive>
    AutoTableDataEntry<Data, Object, Archive>& AutoTableDataEntry<Data, Object, Archive>::operator=(
        const AutoTableDataEntry& arg)
    {
        implementation.reset(arg.implementation->Clone());
        return *this;
    }

    template<class Data, class Object, class Archive>
    AutoTableDataEntry<Data, Object, Archive>& AutoTableDataEntry<Data, Object, Archive>::operator=(
        AutoTableDataEntry&& arg)
    {
        implementation = std::move(arg.implementation);
        return *this;
    }

    template<class Data, class Object, class Archive>
    void AutoTableDataEntry<Data, Object, Archive>::Scriven(TableT& table, ArchiveT& archive)
    {
        implementation->Scriven(table, archive);
    }

    template<class Data, class Object, class Archive>
    void AutoTableDataEntry<Data, Object, Archive>::ObjectScriven(TableT& table, ObjectT& object, ArchiveT& archive)
    {
        implementation->ObjectScriven(table, object, archive);
    }

    template<class Data, class Object, class Archive>
    void AutoTableDataEntry<Data, Object, Archive>::PullFromObject(TableT& table, ObjectT& object)
    {
        implementation->PullFromObject(table, object);
    }

    template<class Data, class Object, class Archive>
    void AutoTableDataEntry<Data, Object, Archive>::PushToObject(TableT& table, ObjectT& object)
    {
        implementation->PushToObject(table, object);
    }

    template<class Data, class Object, class Archive>
    template<class ObjectMemberT, class DataMemberT>
    AutoTableDataEntry<Data, Object, Archive>::AutoTableDataEntry(ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember) :
        implementation(new AutoImplementation<ObjectMemberT, DataMemberT>(objectMember, dataMember))
    {}

    template<class Data, class Object, class Archive>
    template<class BaseT>
    AutoTableDataEntry<Data, Object, Archive>::AutoTableDataEntry(Type<BaseT>) :
        implementation(new BaseImplementation<BaseT>())
    {}

    template<class Data, class Object, class Archive>
    template<class BaseT>
    AutoTableDataEntry<Data, Object, Archive>::AutoTableDataEntry(BaseTableDataEntry<BaseT, ObjectT, ArchiveT>& entry) :
        implementation(new BaseReferenceImplementation<BaseT>(entry))
    {}

    template<class Data, class Object, class Archive>
    AutoTableDataEntry<Data, Object, Archive>::Implementation::~Implementation()
    {}

    template<class Data, class Object, class Archive>
    template<class ObjectMemberT, class DataMemberT>
    AutoTableDataEntry<Data, Object, Archive>::AutoImplementation<ObjectMemberT, DataMemberT>::AutoImplementation(
        ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember) :

        objectMember(objectMember), dataMember(dataMember)
    {}

    template<class Data, class Object, class Archive>
    template<class ObjectMemberT, class DataMemberT>
    AutoTableDataEntry<Data, Object, Archive>::AutoImplementation<ObjectMemberT, DataMemberT>*
        AutoTableDataEntry<Data, Object, Archive>::AutoImplementation<ObjectMemberT, DataMemberT>::Clone() const
    {
        return new AutoImplementation(*this);
    }

    template<class Data, class Object, class Archive>
    template<class ObjectMemberT, class DataMemberT>
    void AutoTableDataEntry<Data, Object, Archive>::AutoImplementation<ObjectMemberT, DataMemberT>::Scriven(
        TableT& table, ArchiveT& archive)
    {
        archive(table.data.*dataMember);
    }

    template<class Data, class Object, class Archive>
    template<class ObjectMemberT, class DataMemberT>
    void AutoTableDataEntry<Data, Object, Archive>::AutoImplementation<ObjectMemberT, DataMemberT>::ObjectScriven(
        TableT& table, ObjectT& object, ArchiveT& archive)
    {}

    template<class Data, class Object, class Archive>
    template<class ObjectMemberT, class DataMemberT>
    void AutoTableDataEntry<Data, Object, Archive>::AutoImplementation<ObjectMemberT, DataMemberT>::PullFromObject(
        TableT& table, ObjectT& object)
    {
        (table.data.*dataMember) = (object.*objectMember);
    }

    template<class Data, class Object, class Archive>
    template<class ObjectMemberT, class DataMemberT>
    void AutoTableDataEntry<Data, Object, Archive>::AutoImplementation<ObjectMemberT, DataMemberT>::PushToObject(
        TableT& table, ObjectT& object)
    {
        (object.*objectMember) = (table.data.*dataMember);
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
        TableT& table, ArchiveT& archive)
    {
        baseEntry.Scriven(archive);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataEntry<Data, Object, Archive>::BaseImplementation<BaseT>::ObjectScriven(
        TableT& table, ObjectT& object, ArchiveT& archive)
    {
        baseEntry.ObjectScriven(object, archive);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataEntry<Data, Object, Archive>::BaseImplementation<BaseT>::PullFromObject(
        TableT& table, ObjectT& object)
    {
        baseEntry.PullFromObject(object);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataEntry<Data, Object, Archive>::BaseImplementation<BaseT>::PushToObject(
        TableT& table, ObjectT& object)
    {
        baseEntry.PushToObject(object);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    AutoTableDataEntry<Data, Object, Archive>::BaseReferenceImplementation<BaseT>::BaseReferenceImplementation(
        BaseEntry& baseEntry) : baseEntry(&baseEntry)
    {}

    template<class Data, class Object, class Archive>
    template<class BaseT>
    AutoTableDataEntry<Data, Object, Archive>::BaseReferenceImplementation<BaseT>*
        AutoTableDataEntry<Data, Object, Archive>::BaseReferenceImplementation<BaseT>::Clone() const
    {
        return new BaseReferenceImplementation(*this);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataEntry<Data, Object, Archive>::BaseReferenceImplementation<BaseT>::Scriven(
        TableT& table, ArchiveT& archive)
    {
        baseEntry->Scriven(archive);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataEntry<Data, Object, Archive>::BaseReferenceImplementation<BaseT>::ObjectScriven(
        TableT& table, ObjectT& object, ArchiveT& archive)
    {
        baseEntry->ObjectScriven(object, archive);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataEntry<Data, Object, Archive>::BaseReferenceImplementation<BaseT>::PullFromObject(
        TableT& table, ObjectT& object)
    {
        baseEntry->PullFromObject(object);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataEntry<Data, Object, Archive>::BaseReferenceImplementation<BaseT>::PushToObject(
        TableT& table, ObjectT& object)
    {
        baseEntry->PushToObject(object);
    }
}