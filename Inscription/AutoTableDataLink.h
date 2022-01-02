#pragma once

#include "BaseTableDataLink.h"
#include "TypeIdentity.h"

#include "ObjectTrackingContext.h"

namespace Inscription
{
    template<class Table, class Object, class Format>
    class AutoTableDataLink
    {
    public:
        using TableT = Table;
        using ObjectT = Object;
        using FormatT = Format;

        using DataT = typename TableT::DataT;
    public:
        template<class ObjectMemberT, class DataMemberT>
        static AutoTableDataLink Auto(ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember);
        template<class BaseT>
        static AutoTableDataLink Base(TypeIdentity<BaseT>);
        template<class BaseT>
        static AutoTableDataLink Base(BaseTableDataLink<BaseT, ObjectT, FormatT>& link);

        AutoTableDataLink(const AutoTableDataLink& arg);
        AutoTableDataLink(AutoTableDataLink&& arg) noexcept;
        
        AutoTableDataLink& operator=(const AutoTableDataLink& arg);
        AutoTableDataLink& operator=(AutoTableDataLink&& arg) noexcept;

        void Scriven(TableT& table, FormatT& format);
        void ObjectScriven(TableT& table, ObjectT& object, FormatT& format);

        void PullFromObject(TableT& table, ObjectT& object, FormatT& format);
        void PushToObject(TableT& table, ObjectT& object, FormatT& format);
    private:
        template<class ObjectMemberT, class DataMemberT>
        AutoTableDataLink(ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember);
        template<class BaseT>
        explicit AutoTableDataLink(TypeIdentity<BaseT> type);
        template<class BaseT>
        explicit AutoTableDataLink(BaseTableDataLink<BaseT, ObjectT, FormatT>& link);
    private:
        class Implementation
        {
        public:
            virtual ~Implementation() = 0;

            [[nodiscard]] virtual Implementation* Clone() const = 0;

            virtual void Scriven(TableT& table, FormatT& format) = 0;
            virtual void ObjectScriven(TableT& table, ObjectT& object, FormatT& format) = 0;

            virtual void PullFromObject(TableT& table, ObjectT& object, FormatT& format) = 0;
            virtual void PushToObject(TableT& table, ObjectT& object, FormatT& format) = 0;
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

            void Scriven(TableT& table, FormatT& format) override;
            void ObjectScriven(TableT& table, ObjectT& object, FormatT& format) override;

            void PullFromObject(TableT& table, ObjectT& object, FormatT& format) override;
            void PushToObject(TableT& table, ObjectT& object, FormatT& format) override;
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

            void Scriven(TableT& table, FormatT& format) override;
            void ObjectScriven(TableT& table, ObjectT& object, FormatT& format) override;

            void PullFromObject(TableT& table, ObjectT& object, FormatT& format) override;
            void PushToObject(TableT& table, ObjectT& object, FormatT& format) override;
        };

        template<class BaseT>
        class BaseImplementation : public Implementation
        {
        public:
            using BaseLink = BaseTableDataLink<BaseT, ObjectT, FormatT>;
            BaseLink baseLink;
        public:
            ~BaseImplementation() = default;

            BaseImplementation* Clone() const override;

            void Scriven(TableT& table, FormatT& format) override;
            void ObjectScriven(TableT& table, ObjectT& object, FormatT& format) override;

            void PullFromObject(TableT& table, ObjectT& object, FormatT& format) override;
            void PushToObject(TableT& table, ObjectT& object, FormatT& format) override;
        };

        template<class BaseT>
        class BaseReferenceImplementation : public Implementation
        {
        public:
            using BaseLink = BaseTableDataLink<BaseT, ObjectT, FormatT>;
            BaseLink* baseLink;
        public:
            explicit BaseReferenceImplementation(BaseLink& baseLink);

            ~BaseReferenceImplementation() = default;

            BaseReferenceImplementation* Clone() const override;

            void Scriven(TableT& table, FormatT& format) override;
            void ObjectScriven(TableT& table, ObjectT& object, FormatT& format) override;

            void PullFromObject(TableT& table, ObjectT& object, FormatT& format) override;
            void PushToObject(TableT& table, ObjectT& object, FormatT& format) override;
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

    template<class Data, class Object, class Format>
    template<class ObjectMemberT, class DataMemberT>
    AutoTableDataLink<Data, Object, Format> AutoTableDataLink<Data, Object, Format>::Auto(
        ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember)
    {
        return AutoTableDataLink(objectMember, dataMember);
    }

    template<class Data, class Object, class Format>
    template<class BaseT>
    AutoTableDataLink<Data, Object, Format> AutoTableDataLink<Data, Object, Format>::Base(
        TypeIdentity<BaseT> type)
    {
        return AutoTableDataLink(type);
    }

    template<class Data, class Object, class Format>
    template<class BaseT>
    AutoTableDataLink<Data, Object, Format> AutoTableDataLink<Data, Object, Format>::Base(
        BaseTableDataLink<BaseT, ObjectT, FormatT>& link)
    {
        return AutoTableDataLink(link);
    }

    template<class Data, class Object, class Format>
    AutoTableDataLink<Data, Object, Format>::AutoTableDataLink(const AutoTableDataLink& arg) :
        implementation(arg.implementation->Clone())
    {}

    template<class Data, class Object, class Format>
    AutoTableDataLink<Data, Object, Format>::AutoTableDataLink(AutoTableDataLink&& arg) noexcept :
        implementation(std::move(arg.implementation))
    {}

    template<class Data, class Object, class Format>
    AutoTableDataLink<Data, Object, Format>& AutoTableDataLink<Data, Object, Format>::operator=(
        const AutoTableDataLink& arg)
    {
        implementation.reset(arg.implementation->Clone());
        return *this;
    }

    template<class Data, class Object, class Format>
    AutoTableDataLink<Data, Object, Format>& AutoTableDataLink<Data, Object, Format>::operator=(
        AutoTableDataLink&& arg) noexcept
    {
        implementation = std::move(arg.implementation);
        return *this;
    }

    template<class Data, class Object, class Format>
    void AutoTableDataLink<Data, Object, Format>::Scriven(TableT& table, FormatT& format)
    {
        implementation->Scriven(table, format);
    }

    template<class Data, class Object, class Format>
    void AutoTableDataLink<Data, Object, Format>::ObjectScriven(TableT& table, ObjectT& object, FormatT& format)
    {
        implementation->ObjectScriven(table, object, format);
    }

    template<class Data, class Object, class Format>
    void AutoTableDataLink<Data, Object, Format>::PullFromObject(TableT& table, ObjectT& object, FormatT& format)
    {
        implementation->PullFromObject(table, object, format);
    }

    template<class Data, class Object, class Format>
    void AutoTableDataLink<Data, Object, Format>::PushToObject(TableT& table, ObjectT& object, FormatT& format)
    {
        implementation->PushToObject(table, object, format);
    }

    template<class Data, class Object, class Format>
    template<class ObjectMemberT, class DataMemberT>
    AutoTableDataLink<Data, Object, Format>::AutoTableDataLink(ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember) :
        implementation(CreateImplementation(objectMember, dataMember))
    {}

    template<class Data, class Object, class Format>
    template<class BaseT>
    AutoTableDataLink<Data, Object, Format>::AutoTableDataLink(TypeIdentity<BaseT> type) :
        implementation(new BaseImplementation<BaseT>())
    {}

    template<class Data, class Object, class Format>
    template<class BaseT>
    AutoTableDataLink<Data, Object, Format>::AutoTableDataLink(BaseTableDataLink<BaseT, ObjectT, FormatT>& link) :
        implementation(new BaseReferenceImplementation<BaseT>(link))
    {}

    template<class Data, class Object, class Format>
    AutoTableDataLink<Data, Object, Format>::Implementation::~Implementation() = default;

    template<class Data, class Object, class Format>
    template<class ObjectMemberT, class DataMemberT>
    AutoTableDataLink<Data, Object, Format>::AutoImplementation<ObjectMemberT, DataMemberT>::AutoImplementation(
        ObjectMemberT ObjectT::*objectMember, DataMemberT DataT::*dataMember) :

        objectMember(objectMember), dataMember(dataMember)
    {}

    template<class Data, class Object, class Format>
    template<class ObjectMemberT, class DataMemberT>
    auto AutoTableDataLink<Data, Object, Format>::AutoImplementation<ObjectMemberT, DataMemberT>::Clone() const
        -> AutoImplementation<ObjectMemberT, DataMemberT>*
    {
        return new AutoImplementation(*this);
    }

    template<class Data, class Object, class Format>
    template<class ObjectMemberT, class DataMemberT>
    void AutoTableDataLink<Data, Object, Format>::AutoImplementation<ObjectMemberT, DataMemberT>::Scriven(
        TableT& table, FormatT& format)
    {
        auto trackingContext = ObjectTrackingContext::Inactive(format.types);
        format(table.data.*dataMember);
    }

    template<class Data, class Object, class Format>
    template<class ObjectMemberT, class DataMemberT>
    void AutoTableDataLink<Data, Object, Format>::AutoImplementation<ObjectMemberT, DataMemberT>::ObjectScriven(
        TableT& table, ObjectT& object, FormatT& format)
    {
        format.types.AttemptTrackObject(&RemoveConst(object.*objectMember));
    }

    template<class Data, class Object, class Format>
    template<class ObjectMemberT, class DataMemberT>
    void AutoTableDataLink<Data, Object, Format>::AutoImplementation<ObjectMemberT, DataMemberT>::PullFromObject(
        TableT& table, ObjectT& object, FormatT& format)
    {
        RemoveConst(table.data.*dataMember) = RemoveConst(object.*objectMember);
    }

    template<class Data, class Object, class Format>
    template<class ObjectMemberT, class DataMemberT>
    void AutoTableDataLink<Data, Object, Format>::AutoImplementation<ObjectMemberT, DataMemberT>::PushToObject(
        TableT& table, ObjectT& object, FormatT& format)
    {
        RemoveConst(object.*objectMember) = std::move(table.data.*dataMember);
    }

    template<class Data, class Object, class Format>
    template<class T>
    AutoTableDataLink<Data, Object, Format>::AutoSameTypeImplementation<T>::AutoSameTypeImplementation(
        T ObjectT::*objectMember, T DataT::*dataMember)
        :
        objectMember(objectMember), dataMember(dataMember)
    {}

    template<class Data, class Object, class Format>
    template<class T>
    auto AutoTableDataLink<Data, Object, Format>::AutoSameTypeImplementation<T>::Clone() const
        -> AutoSameTypeImplementation<T>*
    {
        return new AutoSameTypeImplementation(*this);
    }

    template<class Data, class Object, class Format>
    template<class T>
    void AutoTableDataLink<Data, Object, Format>::AutoSameTypeImplementation<T>::Scriven(
        TableT& table, FormatT& format)
    {
        auto trackingContext = ObjectTrackingContext::Inactive(format.types);
        format(table.data.*dataMember);
    }

    template<class Data, class Object, class Format>
    template<class T>
    void AutoTableDataLink<Data, Object, Format>::AutoSameTypeImplementation<T>::ObjectScriven(
        TableT& table, ObjectT& object, FormatT& format)
    {
        format.types.AttemptTrackObject(&RemoveConst(object.*objectMember));
    }

    template<class Data, class Object, class Format>
    template<class T>
    void AutoTableDataLink<Data, Object, Format>::AutoSameTypeImplementation<T>::PullFromObject(
        TableT& table, ObjectT& object, FormatT& format)
    {
        RemoveConst(table.data.*dataMember) = object.*objectMember;
    }

    template<class Data, class Object, class Format>
    template<class T>
    void AutoTableDataLink<Data, Object, Format>::AutoSameTypeImplementation<T>::PushToObject(
        TableT& table, ObjectT& object, FormatT& format)
    {
        RemoveConst(object.*objectMember) = std::move(table.data.*dataMember);
    }

    template<class Data, class Object, class Format>
    template<class T>
    auto AutoTableDataLink<Data, Object, Format>::BaseImplementation<T>::Clone() const
        -> BaseImplementation<T>*
    {
        return new BaseImplementation(*this);
    }

    template<class Data, class Object, class Format>
    template<class BaseT>
    void AutoTableDataLink<Data, Object, Format>::BaseImplementation<BaseT>::Scriven(
        TableT& table, FormatT& format)
    {
        baseLink.Scriven(format);
    }

    template<class Data, class Object, class Format>
    template<class BaseT>
    void AutoTableDataLink<Data, Object, Format>::BaseImplementation<BaseT>::ObjectScriven(
        TableT& table, ObjectT& object, FormatT& format)
    {
        baseLink.ObjectScriven(object, format);
    }

    template<class Data, class Object, class Format>
    template<class BaseT>
    void AutoTableDataLink<Data, Object, Format>::BaseImplementation<BaseT>::PullFromObject(
        TableT& table, ObjectT& object, FormatT& format)
    {
        baseLink.PullFromObject(object, format);
    }

    template<class Data, class Object, class Format>
    template<class BaseT>
    void AutoTableDataLink<Data, Object, Format>::BaseImplementation<BaseT>::PushToObject(
        TableT& table, ObjectT& object, FormatT& format)
    {
        baseLink.PushToObject(object, format);
    }

    template<class Data, class Object, class Format>
    template<class BaseT>
    AutoTableDataLink<Data, Object, Format>::BaseReferenceImplementation<BaseT>::BaseReferenceImplementation(
        BaseLink& baseLink) : baseLink(&baseLink)
    {}

    template<class Data, class Object, class Format>
    template<class BaseT>
    auto AutoTableDataLink<Data, Object, Format>::BaseReferenceImplementation<BaseT>::Clone() const
        -> BaseReferenceImplementation<BaseT>*
    {
        return new BaseReferenceImplementation(*this);
    }

    template<class Data, class Object, class Format>
    template<class BaseT>
    void AutoTableDataLink<Data, Object, Format>::BaseReferenceImplementation<BaseT>::Scriven(
        TableT& table, FormatT& format)
    {
        baseLink->Scriven(format);
    }

    template<class Data, class Object, class Format>
    template<class BaseT>
    void AutoTableDataLink<Data, Object, Format>::BaseReferenceImplementation<BaseT>::ObjectScriven(
        TableT& table, ObjectT& object, FormatT& format)
    {
        baseLink->ObjectScriven(object, format);
    }

    template<class Data, class Object, class Format>
    template<class BaseT>
    void AutoTableDataLink<Data, Object, Format>::BaseReferenceImplementation<BaseT>::PullFromObject(
        TableT& table, ObjectT& object, FormatT& format)
    {
        baseLink->PullFromObject(object, format);
    }

    template<class Data, class Object, class Format>
    template<class BaseT>
    void AutoTableDataLink<Data, Object, Format>::BaseReferenceImplementation<BaseT>::PushToObject(
        TableT& table, ObjectT& object, FormatT& format)
    {
        baseLink->PushToObject(object, format);
    }
}