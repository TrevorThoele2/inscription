#include "StreamPositionScribe.h"

#include "NumericScribe.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"

namespace Inscription
{
    void Scribe<StreamPosition>::Scriven(ObjectT& object, BinaryArchive& archive)
    {
        auto trackingContext = ObjectTrackingContext::Inactive(archive.types);
        if (archive.IsOutput())
        {
            unsigned long long converted = object;
            archive(converted);
        }
        else
        {
            unsigned long long loaded;
            archive(loaded);
            object = ObjectT(loaded);
        }
    }
}