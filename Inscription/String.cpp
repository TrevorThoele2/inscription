#include "String.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"

#include "ContainerSize.h"
#include "ScopeTrackingModifier.h"
#include "Const.h"

namespace Inscription
{
    void Scribe<std::string, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    void Scribe<std::string, BinaryArchive>::SaveImplementation(ObjectT& object, ArchiveT& archive)
    {
        ScopeTrackingModifier tracking(archive, false);

        ContainerSize size(object.size());
        archive(size);
        for (auto& loop : object)
            archive(loop);
    }

    void Scribe<std::string, BinaryArchive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
    {
        ScopeTrackingModifier tracking(archive, false);

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