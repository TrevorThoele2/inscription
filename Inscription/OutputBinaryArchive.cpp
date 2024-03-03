#include "OutputBinaryArchive.h"

#include "ContainerSize.h"
#include "Const.h"

namespace Inscription
{
    OutputBinaryArchive::OutputBinaryArchive(
        const FilePath& path,
        const Signature& signature,
        Version version) :

        BinaryArchive(Direction::OUTPUT, signature, version), file(path)
    {
        InitialSetup();
    }

    OutputBinaryArchive::OutputBinaryArchive(
        const FilePath& path,
        const Signature& signature,
        Version version,
        const TypeRegistrationContext& typeRegistrationContext) :

        BinaryArchive(Direction::OUTPUT, signature, version, typeRegistrationContext), file(path)
    {
        InitialSetup();
    }

    OutputBinaryArchive::OutputBinaryArchive(OutputBinaryArchive&& arg) :
        BinaryArchive(std::move(arg)), file(std::move(arg.file))
    {}

    OutputBinaryArchive& OutputBinaryArchive::operator=(OutputBinaryArchive&& arg)
    {
        BinaryArchive::operator=(std::move(arg));
        file = std::move(arg.file);
        return *this;
    }

    void OutputBinaryArchive::SeekStream(StreamPosition position)
    {
        file.SeekStream(position);
    }

    OutputBinaryArchive::StreamPosition OutputBinaryArchive::TellStream()
    {
        return file.TellStream();
    }

    void OutputBinaryArchive::InitialSetup()
    {
        for (auto& loop : signature)
            WriteImpl(RemoveConst(loop));

        WriteImpl(version);
        postHeaderPosition = TellStream();
    }
}