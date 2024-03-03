#include "StringScribe.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"

#include "OutputTextArchive.h"
#include "InputTextArchive.h"

#include "ContainerSize.h"
#include "ObjectTrackingContext.h"
#include "Const.h"

namespace Inscription
{
    void Scribe<std::string, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        auto trackingContext = ObjectTrackingContext::Inactive(archive.types);
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    void Scribe<std::string, BinaryArchive>::SaveImplementation(ObjectT& object, ArchiveT& archive)
    {
        ContainerSize size(object.size());
        archive(size);
        for (auto& loop : object)
            archive(loop);
    }

    void Scribe<std::string, BinaryArchive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
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

    void Scribe<std::string, JsonArchive>::ScrivenImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        auto trackingContext = ObjectTrackingContext::Inactive(archive.types);
        if (archive.IsOutput())
            SaveImplementation(name, object, archive);
        else
            LoadImplementation(name, object, archive);
    }

    void Scribe<std::string, JsonArchive>::SaveImplementation(
        const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        std::string output;
        for(auto& character : object)
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

    void Scribe<std::string, JsonArchive>::LoadImplementation(
        const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        std::string value;
        archive.AsInput()->ReadValue(name, value);
        object = value.substr(1, value.size() - 2);
    }

    void Scribe<std::string, TextArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    void Scribe<std::string, TextArchive>::SaveImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive.AsOutput()->Write(object);
    }

    void Scribe<std::string, TextArchive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
    {
        archive.AsInput()->ReadLine(object);
    }
}