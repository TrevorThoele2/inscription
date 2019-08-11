#include "ObjectTrackingContext.h"

#include "BinaryArchive.h"

namespace Inscription
{
    ObjectTrackingContext::ObjectTrackingContext(Type type, BinaryArchive& archive) :
        archive(&archive), previousType(TypeFrom(archive.TrackObjects(BoolFrom(type))))
    {}

    ObjectTrackingContext::~ObjectTrackingContext()
    {
        archive->TrackObjects(BoolFrom(previousType));
    }

    bool ObjectTrackingContext::BoolFrom(Type type)
    {
        return (type == Active) ? true : false;
    }

    auto ObjectTrackingContext::TypeFrom(bool b) -> Type
    {
        return (b) ? Active : Inactive;
    }
}