#pragma once

#include "BaseTableDataLink.h"
#include "Type.h"

#include "ObjectTrackingContext.h"

namespace Inscription
{
    template<class Table, class Object, class Archive>
    class AutoTableDataLink
    {
    public:
        using TableT = Table;
        using ObjectT = Object;
        using ArchiveT = Archive;

        using DataT = typename TableT::DataT;
    public:
        template<class ObjectMemberT, class DataMemberT>
        static AutoTableDataLink Auto(ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember);
        template<class BaseT>
        static AutoTableDataLink Base(Type<BaseT>);
        template<class BaseT>
        static AutoTableDataLink Base(BaseTableDataLink<BaseT, ObjectT, ArchiveT>& link);

        AutoTableDataLink(const AutoTableDataLink& arg);
        AutoTableDataLink(AutoTableDataLink&& arg) noexcept;
        
        AutoTableDataLink& operator=(const AutoTableDataLink& arg);
        AutoTableDataLink& operator=(AutoTableDataLink&& arg) noexcept;

        void Scriven(TableT& table, ArchiveT& archive);
        void ObjectScriven(TableT& table, ObjectT& object, ArchiveT& archive);

        void PullFromObject(TableT& table, ObjectT& object, ArchiveT& archive);
        void PushToObject(TableT& table, ObjectT& object, ArchiveT& archive);
    private:
        template<class ObjectMemberT, class DataMemberT>
        AutoTableDataLink(ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember);
        template<class BaseT>
        explicit AutoTableDataLink(Type<BaseT> type);
        template<class BaseT>
        explicit AutoTableDataLink(BaseTableDataLink<BaseT, ObjectT, ArchiveT>& link);
    private:
        class Implementation
        {
        public:
            virtual ~Implementation() = 0;

            [[nodiscard]] virtual Implementation* Clone() const = 0;

            virtual void Scriven(TableT& table, ArchiveT& archive) = 0;
            virtual void ObjectScriven(TableT& table, ObjectT& object, ArchiveT& archive) = 0;

            virtual void PullFromObject(TableT& table, ObjectT& object, ArchiveT& archive) = 0;
            virtual void PushToObject(TableT& table, ObjectT& object, ArchiveT& archive) = 0;
        };

        template<class ObjectMemberT, class DataMemberT>
        class AutoImplementation : public Implementation
        {
        public:
            ObjectMemberT ObjectT::*objectMember;
            DataMemberT DataT::*dataMember;
        public:
            AutoImplementation(ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember);

            ~AutoImplementation() = default;

            AutoImplementation* Clone() const override;

            void Scriven(TableT& table, ArchiveT& archive) override;
            void ObjectScriven(TableT& table, ObjectT& object, ArchiveT& archive) override;

            void PullFromObject(TableT& table, ObjectT& object, ArchiveT& archive) override;
            void PushToObject(TableT& table, ObjectT& object, ArchiveT& archive) override;
        };

        template<class T>
        class AutoSameTypeImplementation : public Implementation
        {
        public:
            T ObjectT::*objectMember;
            T DataT::*dataMember;
        public:
            AutoSameTypeImplementation(T ObjectT::*objectMember, T DataT::*dataMember);

            ~AutoSameTypeImplementation() = default;

            AutoSameTypeImplementation* Clone() const override;

            void Scriven(TableT& table, ArchiveT& archive) override;
            void ObjectScriven(TableT& table, ObjectT& object, ArchiveT& archive) override;

            void PullFromObject(TableT& table, ObjectT& object, ArchiveT& archive) override;
            void PushToObject(TableT& table, ObjectT& object, ArchiveT& archive) override;
        };

        template<class BaseT>
        class BaseImplementation : public Implementation
        {
        public:
            using BaseLink = BaseTableDataLink<BaseT, ObjectT, ArchiveT>;
            BaseLink baseLink;
        public:
            ~BaseImplementation() = default;

            BaseImplementation* Clone() const override;

            void Scriven(TableT& table, ArchiveT& archive) override;
            void ObjectScriven(TableT& table, ObjectT& object, ArchiveT& archive) override;

            void PullFromObject(TableT& table, ObjectT& object, ArchiveT& archive) override;
            void PushToObject(TableT& table, ObjectT& object, ArchiveT& archive) override;
        };

        template<class BaseT>
        class BaseReferenceImplementation : public Implementation
        {
        public:
            using BaseLink = BaseTableDataLink<BaseT, ObjectT, ArchiveT>;
            BaseLink* baseLink;
        public:
            explicit BaseReferenceImplementation(BaseLink& baseLink);

            ~BaseReferenceImplementation() = default;

            BaseReferenceImplementation* Clone() const override;

            void Scriven(TableT& table, ArchiveT& archive) override;
            void ObjectScriven(TableT& table, ObjectT& object, ArchiveT& archive) override;

            void PullFromObject(TableT& table, ObjectT& object, ArchiveT& archive) override;
            void PushToObject(TableT& table, ObjectT& object, ArchiveT& archive) override;
        };

        using ImplementationPtr = std::unique_ptr<Implementation>;
        ImplementationPtr implementation;

        template<class ObjectMemberT, class DataMemberT, std::enable_if_t<!std::is_same_v<ObjectMemberT, DataMemberT>, int> = 0>
        static ImplementationPtr CreateImplementation(ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember)
        {
            return ImplementationPtr(new AutoImplementation<ObjectMemberT, DataMemberT>(objectMember, dataMember));
        }

        template<class ObjectMemberT, class DataMemberT, std::enable_if_t<std::is_same_v<ObjectMemberT, DataMemberT>, int> = 0>
        static ImplementationPtr CreateImplementation(ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember)
        {
            return ImplementationPtr(new AutoSameTypeImplementation<ObjectMemberT>(objectMember, dataMember));
        }
    };

    template<class Data, class Object, class Archive>
    template<class ObjectMemberT, class DataMemberT>
    AutoTableDataLink<Data, Object, Archive> AutoTableDataLink<Data, Object, Archive>::Auto(
        ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember)
    {
        return AutoTableDataLink(objectMember, dataMember);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    AutoTableDataLink<Data, Object, Archive> AutoTableDataLink<Data, Object, Archive>::Base(
        Type<BaseT> type)
    {
        return AutoTableDataLink(type);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    AutoTableDataLink<Data, Object, Archive> AutoTableDataLink<Data, Object, Archive>::Base(
        BaseTableDataLink<BaseT, ObjectT, ArchiveT>& link)
    {
        return AutoTableDataLink(link);
    }

    template<class Data, class Object, class Archive>
    AutoTableDataLink<Data, Object, Archive>::AutoTableDataLink(const AutoTableDataLink& arg) :
        implementation(arg.implementation->Clone())
    {}

    template<class Data, class Object, class Archive>
    AutoTableDataLink<Data, Object, Archive>::AutoTableDataLink(AutoTableDataLink&& arg) noexcept :
        implementation(std::move(arg.implementation))
    {}

    template<class Data, class Object, class Archive>
    AutoTableDataLink<Data, Object, Archive>& AutoTableDataLink<Data, Object, Archive>::operator=(
        const AutoTableDataLink& arg)
    {
        implementation.reset(arg.implementation->Clone());
        return *this;
    }

    template<class Data, class Object, class Archive>
    AutoTableDataLink<Data, Object, Archive>& AutoTableDataLink<Data, Object, Archive>::operator=(
        AutoTableDataLink&& arg) noexcept
    {
        implementation = std::move(arg.implementation);
        return *this;
    }

    template<class Data, class Object, class Archive>
    void AutoTableDataLink<Data, Object, Archive>::Scriven(TableT& table, ArchiveT& archive)
    {
        implementation->Scriven(table, archive);
    }

    template<class Data, class Object, class Archive>
    void AutoTableDataLink<Data, Object, Archive>::ObjectScriven(TableT& table, ObjectT& object, ArchiveT& archive)
    {
        implementation->ObjectScriven(table, object, archive);
    }

    template<class Data, class Object, class Archive>
    void AutoTableDataLink<Data, Object, Archive>::PullFromObject(TableT& table, ObjectT& object, ArchiveT& archive)
    {
        implementation->PullFromObject(table, object, archive);
    }

    template<class Data, class Object, class Archive>
    void AutoTableDataLink<Data, Object, Archive>::PushToObject(TableT& table, ObjectT& object, ArchiveT& archive)
    {
        implementation->PushToObject(table, object, archive);
    }

    template<class Data, class Object, class Archive>
    template<class ObjectMemberT, class DataMemberT>
    AutoTableDataLink<Data, Object, Archive>::AutoTableDataLink(ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember) :
        implementation(CreateImplementation(objectMember, dataMember))
    {}

    template<class Data, class Object, class Archive>
    template<class BaseT>
    AutoTableDataLink<Data, Object, Archive>::AutoTableDataLink(Type<BaseT> type) :
        implementation(new BaseImplementation<BaseT>())
    {}

    template<class Data, class Object, class Archive>
    template<class BaseT>
    AutoTableDataLink<Data, Object, Archive>::AutoTableDataLink(BaseTableDataLink<BaseT, ObjectT, ArchiveT>& link) :
        implementation(new BaseReferenceImplementation<BaseT>(link))
    {}

    template<class Data, class Object, class Archive>
    AutoTableDataLink<Data, Object, Archive>::Implementation::~Implementation() = default;

    template<class Data, class Object, class Archive>
    template<class ObjectMemberT, class DataMemberT>
    AutoTableDataLink<Data, Object, Archive>::AutoImplementation<ObjectMemberT, DataMemberT>::AutoImplementation(
        ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember) :

        objectMember(objectMember), dataMember(dataMember)
    {}

    template<class Data, class Object, class Archive>
    template<class ObjectMemberT, class DataMemberT>
    auto AutoTableDataLink<Data, Object, Archive>::AutoImplementation<ObjectMemberT, DataMemberT>::Clone() const
        -> AutoImplementation<ObjectMemberT, DataMemberT>*
    {
        return new AutoImplementation(*this);
    }

    template<class Data, class Object, class Archive>
    template<class ObjectMemberT, class DataMemberT>
    void AutoTableDataLink<Data, Object, Archive>::AutoImplementation<ObjectMemberT, DataMemberT>::Scriven(
        TableT& table, ArchiveT& archive)
    {
        ObjectTrackingContext trackingContext(ObjectTrackingContext::Inactive, archive);
        archive(table.data.*dataMember);
    }

    template<class Data, class Object, class Archive>
    template<class ObjectMemberT, class DataMemberT>
    void AutoTableDataLink<Data, Object, Archive>::AutoImplementation<ObjectMemberT, DataMemberT>::ObjectScriven(
        TableT& table, ObjectT& object, ArchiveT& archive)
    {
        archive.AttemptTrackObject(&RemoveConst(object.*objectMember));
    }

    template<class Data, class Object, class Archive>
    template<class ObjectMemberT, class DataMemberT>
    void AutoTableDataLink<Data, Object, Archive>::AutoImplementation<ObjectMemberT, DataMemberT>::PullFromObject(
        TableT& table, ObjectT& object, ArchiveT& archive)
    {
        auto& extractedDataMember = RemoveConst(table.data.*dataMember); 
        auto extractedObjectMember = RemoveConst(object.*objectMember);
        extractedDataMember = extractedObjectMember;
    }

    template<class Data, class Object, class Archive>
    template<class ObjectMemberT, class DataMemberT>
    void AutoTableDataLink<Data, Object, Archive>::AutoImplementation<ObjectMemberT, DataMemberT>::PushToObject(
        TableT& table, ObjectT& object, ArchiveT& archive)
    {
        auto& extractedObjectMember = RemoveConst(object.*objectMember);
        std::remove_const_t<ObjectMemberT> extractedDataMember(table.data.*dataMember);
        extractedObjectMember = std::move(extractedDataMember);
    }

    template<class Data, class Object, class Archive>
    template<class T>
    AutoTableDataLink<Data, Object, Archive>::AutoSameTypeImplementation<T>::AutoSameTypeImplementation(
        T ObjectT::*objectMember, T DataT::*dataMember)
        :
        objectMember(objectMember), dataMember(dataMember)
    {}

    template<class Data, class Object, class Archive>
    template<class T>
    auto AutoTableDataLink<Data, Object, Archive>::AutoSameTypeImplementation<T>::Clone() const
        -> AutoSameTypeImplementation<T>*
    {
        return new AutoSameTypeImplementation(*this);
    }

    template<class Data, class Object, class Archive>
    template<class T>
    void AutoTableDataLink<Data, Object, Archive>::AutoSameTypeImplementation<T>::Scriven(
        TableT& table, ArchiveT& archive)
    {
        ObjectTrackingContext trackingContext(ObjectTrackingContext::Inactive, archive);
        archive(table.data.*dataMember);
    }

    template<class Data, class Object, class Archive>
    template<class T>
    void AutoTableDataLink<Data, Object, Archive>::AutoSameTypeImplementation<T>::ObjectScriven(
        TableT& table, ObjectT& object, ArchiveT& archive)
    {
        archive.AttemptTrackObject(&RemoveConst(object.*objectMember));
    }

    template<class Data, class Object, class Archive>
    template<class T>
    void AutoTableDataLink<Data, Object, Archive>::AutoSameTypeImplementation<T>::PullFromObject(
        TableT& table, ObjectT& object, ArchiveT& archive)
    {
        auto& extractedDataMember = RemoveConst(table.data.*dataMember);
        auto& extractedObjectMember = (object.*objectMember);
        extractedDataMember = extractedObjectMember;
    }

    template<class Data, class Object, class Archive>
    template<class T>
    void AutoTableDataLink<Data, Object, Archive>::AutoSameTypeImplementation<T>::PushToObject(
        TableT& table, ObjectT& object, ArchiveT& archive)
    {
        auto& extractedObjectMember = RemoveConst((object.*objectMember));
        auto& extractedDataMember = (table.data.*dataMember);
        extractedObjectMember = std::move(extractedDataMember);
    }

    template<class Data, class Object, class Archive>
    template<class T>
    auto AutoTableDataLink<Data, Object, Archive>::BaseImplementation<T>::Clone() const
        -> BaseImplementation<T>*
    {
        return new BaseImplementation(*this);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataLink<Data, Object, Archive>::BaseImplementation<BaseT>::Scriven(
        TableT& table, ArchiveT& archive)
    {
        baseLink.Scriven(archive);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataLink<Data, Object, Archive>::BaseImplementation<BaseT>::ObjectScriven(
        TableT& table, ObjectT& object, ArchiveT& archive)
    {
        baseLink.ObjectScriven(object, archive);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataLink<Data, Object, Archive>::BaseImplementation<BaseT>::PullFromObject(
        TableT& table, ObjectT& object, ArchiveT& archive)
    {
        baseLink.PullFromObject(object, archive);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataLink<Data, Object, Archive>::BaseImplementation<BaseT>::PushToObject(
        TableT& table, ObjectT& object, ArchiveT& archive)
    {
        baseLink.PushToObject(object, archive);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    AutoTableDataLink<Data, Object, Archive>::BaseReferenceImplementation<BaseT>::BaseReferenceImplementation(
        BaseLink& baseLink) : baseLink(&baseLink)
    {}

    template<class Data, class Object, class Archive>
    template<class BaseT>
    auto AutoTableDataLink<Data, Object, Archive>::BaseReferenceImplementation<BaseT>::Clone() const
        -> BaseReferenceImplementation<BaseT>*
    {
        return new BaseReferenceImplementation(*this);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataLink<Data, Object, Archive>::BaseReferenceImplementation<BaseT>::Scriven(
        TableT& table, ArchiveT& archive)
    {
        baseLink->Scriven(archive);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataLink<Data, Object, Archive>::BaseReferenceImplementation<BaseT>::ObjectScriven(
        TableT& table, ObjectT& object, ArchiveT& archive)
    {
        baseLink->ObjectScriven(object, archive);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataLink<Data, Object, Archive>::BaseReferenceImplementation<BaseT>::PullFromObject(
        TableT& table, ObjectT& object, ArchiveT& archive)
    {
        baseLink->PullFromObject(object, archive);
    }

    template<class Data, class Object, class Archive>
    template<class BaseT>
    void AutoTableDataLink<Data, Object, Archive>::BaseReferenceImplementation<BaseT>::PushToObject(
        TableT& table, ObjectT& object, ArchiveT& archive)
    {
        baseLink->PushToObject(object, archive);
    }
}