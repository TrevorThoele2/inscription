#include "OutputBinaryArchive.h"

#include "ContainerSize.h"
#include "Const.h"

namespace Inscription
{
    OutputBinaryArchive::OutputBinaryArchive(
        const FilePath& path,
        const Signature& clientSignature,
        Version clientVersion) :

        BinaryArchive(
            Direction::OUTPUT,
            clientSignature,
            clientVersion,
            ::Inscription::currentInscriptionVersion),
        file(path)
    {
        InitialSetup();
    }

    OutputBinaryArchive::OutputBinaryArchive(
        const FilePath& path,
        const Signature& clientSignature,
        Version clientVersion,
        const TypeRegistrationContext& typeRegistrationContext) :

        BinaryArchive(
            Direction::OUTPUT,
            clientSignature,
            clientVersion,
            ::Inscription::currentInscriptionVersion,
            typeRegistrationContext),
        file(path)
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
        for (auto& loop : clientSignature)
            WriteImpl(RemoveConst(loop));

        WriteImpl(inscriptionVersion);
        WriteImpl(clientVersion);
        postHeaderPosition = TellStream();
    }
}