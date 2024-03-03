#pragma once

#include "CompositeScribe.h"

#include "TableBase.h"

namespace Inscription
{
    class BinaryArchive;

    template<class Object, class Archive>
    class TableScribe;

    template<class Object>
    class TableScribe<Object, BinaryArchive> : public CompositeScribe<Object, BinaryArchive>
    {
    private:
        using BaseScribeT = CompositeScribe<Object, BinaryArchive>;
    public:
        using ObjectT = typename BaseScribeT::ObjectT;
        using ArchiveT = typename BaseScribeT::ArchiveT;
        using ClassNameResolver = typename BaseScribeT::ClassNameResolver;
    public:
        void Scriven(ObjectT& object, ArchiveT& archive) override final;
        void Construct(ObjectT* storage, ArchiveT& archive);
    public:
        using TableBase = TableBase<ObjectT, ArchiveT>;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override final;
    private:
        static_assert(std::is_class_v<ObjectT>,
            "The Object given to a TableScribe was not a composite.");
    };

    template<class Object>
    void TableScribe<Object, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        
        {
            auto trackingID = archive.AttemptTrackObject(&object);
            if (trackingID.IsValid())
                archive.TrackSavedConstruction(*trackingID);
        }

        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Active, archive);
            using TableT = typename Scribe<ObjectT, ArchiveT>::Table;

            TableT table;
            if (archive.IsOutput())
            {
                table.PullFromObject(object, archive);
                table.Scriven(archive);
                table.ObjectScriven(object, archive);
            }
            else
            {
                table.Scriven(archive);
                table.PushToObject(object, archive);
                table.ObjectScriven(object, archive);
            }
        }
    }

    template<class Object>
    void TableScribe<Object, BinaryArchive>::Construct(ObjectT* storage, ArchiveT& archive)
    {
        {
            archive.AttemptTrackObject(storage);
        }

        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Active, archive);
            using TableT = typename Scribe<ObjectT, ArchiveT>::Table;

            TableT table;
            table.Scriven(archive);
            table.Construct(storage, archive);
            table.PushToObject(*storage, archive);
            table.ObjectScriven(*storage, archive);
        }
    }

    template<class Object>
    void TableScribe<Object, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {}
}