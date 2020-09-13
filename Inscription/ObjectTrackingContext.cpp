#include "ObjectTrackingContext.h"

#include "TypeManager.h"

namespace Inscription
{
    ObjectTrackingContext ObjectTrackingContext::Active(TypeManagerBase& typeManager)
    {
        return ObjectTrackingContext(Type::Active, typeManager);
    }

    ObjectTrackingContext ObjectTrackingContext::Inactive(TypeManagerBase& typeManager)
    {
        return ObjectTrackingContext(Type::Inactive, typeManager);
    }

    ObjectTrackingContext::~ObjectTrackingContext()
    {
        typeManager->TrackObjects(BoolFrom(previousType));
    }

    ObjectTrackingContext::ObjectTrackingContext(Type type, TypeManagerBase& typeManager) :
        previousType(TypeFrom(typeManager.TrackObjects(BoolFrom(type)))), typeManager(&typeManager)
    {}

    bool ObjectTrackingContext::BoolFrom(Type type)
    {
        return type == Type::Active ? true : false;
    }

    auto ObjectTrackingContext::TypeFrom(bool b) -> Type
    {
        return b ? Type::Active : Type::Inactive;
    }
}