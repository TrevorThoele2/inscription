#include "StringScribe.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"

#include "OutputTextArchive.h"
#include "InputTextArchive.h"

#include "ContainerSize.h"
#include "ObjectTrackingContext.h"
#include "Const.h"

#include "RequiresScribe.h"

namespace Inscription
{
    void Scribe<std::string>::Scriven(ObjectT& object, BinaryArchive& archive)
    {
        auto trackingContext = ObjectTrackingContext::Inactive(archive.types);
        if (archive.IsOutput())
        {
            ContainerSize size(object.size());
            archive(size);
            for (auto& loop : object)
                archive(loop);
        }
        else
        {
            ContainerSize size;
            archive(size);

            object.resize(size);

            ContainerSize address = 0;
            while (address < size)
            {
                archive(object[address]);
                ++address;
            }
        }
    }

    void Scribe<std::string>::Scriven(const std::string& name, ObjectT& object, JsonArchive& archive)
    {
        auto trackingContext = ObjectTrackingContext::Inactive(archive.types);
        if (archive.IsOutput())
        {
            std::string output;
            for (auto& character : object)
            {
                switch (character)
                {
                case '\"':
                case '\\':
                    output += '\\';
                default:
                    output += character;
                }
            }
            archive.AsOutput()->WriteValue(name, "\"" + output + "\"");
        }
        else
        {
            std::string value;
            archive.AsInput()->TakeValue(name, value);
            object = value.substr(1, value.size() - 2);
        }
    }

    void Scribe<std::string>::Scriven(ObjectT& object, TextArchive& archive)
    {
        if (archive.IsOutput())
            archive.AsOutput()->Write(object);
        else
            archive.AsInput()->ReadLine(object);
    }
}