#pragma once

namespace Inscription
{
    class TypeManagerBase;

    class ObjectTrackingContext
    {
    public:
        static ObjectTrackingContext Active(TypeManagerBase& typeManager);
        static ObjectTrackingContext Inactive(TypeManagerBase& typeManager);

        ObjectTrackingContext(const ObjectTrackingContext& arg) = default;
        ~ObjectTrackingContext();

        ObjectTrackingContext& operator=(const ObjectTrackingContext& arg) = default;
    private:
        enum class Type
        {
            Active,
            Inactive
        };

        Type previousType;
        TypeManagerBase* typeManager;

        ObjectTrackingContext(Type type, TypeManagerBase& typeManager);

        static bool BoolFrom(Type type);
        static Type TypeFrom(bool b);
    };
}