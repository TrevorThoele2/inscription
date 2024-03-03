#pragma once

namespace Inscription
{
    class BinaryArchive;

    class ObjectTrackingContext
    {
    public:
        enum Type
        {
            Active,
            Inactive
        };
    public:
        ObjectTrackingContext(Type type, BinaryArchive& archive);
        ~ObjectTrackingContext();
    private:
        Type previousType;
        BinaryArchive* archive;

        static bool BoolFrom(Type type);
        static Type TypeFrom(bool b);
    };
}