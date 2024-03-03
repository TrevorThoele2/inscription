#include "StreamPositionScribe.h"

#include "NumericScribe.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"

namespace Inscription
{
    void Scribe<StreamPosition, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        ObjectTrackingContext trackingContext(ObjectTrackingContext::Inactive, archive);
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    void Scribe<StreamPosition, BinaryArchive>::SaveImplementation(ObjectT& object, ArchiveT& archive)
    {
        unsigned long long converted = object;
        archive(converted);
    }

    void Scribe<StreamPosition, BinaryArchive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
    {
        unsigned long long loaded;
        archive(loaded);
        object = ObjectT(loaded);
    }
}