#include "FilePositionScribe.h"

#include "NumericScribe.h"

#include "OutputBinaryFormat.h"
#include "InputBinaryFormat.h"

namespace Inscription
{
    void Scribe<File::Position>::Scriven(ObjectT& object, Format::Binary& format)
    {
        auto trackingContext = ObjectTrackingContext::Inactive(format.types);
        if (format.IsOutput())
        {
            unsigned long long converted = object;
            format(converted);
        }
        else
        {
            unsigned long long loaded;
            format(loaded);
            object = ObjectT(loaded);
        }
    }
}