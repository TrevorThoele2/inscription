#include "StringScribe.h"

#include "OutputPlaintextFormat.h"
#include "InputPlaintextFormat.h"

#include "ContainerSize.h"
#include "ObjectTrackingContext.h"
#include "Const.h"
#include "EscapeJson.h"

#include "RequiresScribe.h"

namespace Inscription
{
    void Scribe<std::string>::Scriven(ObjectT& object, Format::Binary& format)
    {
        auto trackingContext = ObjectTrackingContext::Inactive(format.types);
        if (format.IsOutput())
        {
            ContainerSize size(object.size());
            format(size);
            for (auto& loop : object)
                format(loop);
        }
        else
        {
            ContainerSize size;
            format(size);

            object.resize(size);

            ContainerSize address = 0;
            while (address < size)
            {
                format(object[address]);
                ++address;
            }
        }
    }

    void Scribe<std::string>::Scriven(const std::string& name, ObjectT& object, Format::Json& format)
    {
        auto trackingContext = ObjectTrackingContext::Inactive(format.types);
        if (format.IsOutput())
            format.AsOutput()->WriteValue(name, "\"" + Format::EscapeJson(object) + "\"");
        else
        {
            std::string value;
            format.AsInput()->TakeValue(name, value);
            object = value.substr(1, value.size() - 2);
        }
    }

    void Scribe<std::string>::Scriven(ObjectT& object, Format::Plaintext& format)
    {
        if (format.IsOutput())
            format.AsOutput()->Write(object);
        else
            format.AsInput()->ReadSize(object);
    }
}