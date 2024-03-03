#include "InputBinaryArchive.h"

#include "ContainerSize.h"

#include "InvalidSignature.h"

namespace Inscription
{
    InputBinaryArchive::InputBinaryArchive(
        const FilePath& path,
        const Signature& signature) :

        BinaryArchive(
            Direction::Input,
            signature,
            0,
            0),
        file(path)
    {
        InitialSetup();
    }

    InputBinaryArchive::InputBinaryArchive(
        const FilePath& path,
        const Signature& signature,
        const TypeRegistrationContext& typeRegistrationContext) :

        BinaryArchive(
            Direction::Input,
            signature,
            0,
            0,
            typeRegistrationContext),
        file(path)
    {
        InitialSetup();
    }

    InputBinaryArchive::InputBinaryArchive(InputBinaryArchive&& arg) noexcept :
        BinaryArchive(std::move(arg)), file(std::move(arg.file))
    {}

    InputBinaryArchive& InputBinaryArchive::operator=(InputBinaryArchive&& arg) noexcept
    {
        BinaryArchive::operator=(std::move(arg));
        file = std::move(arg.file);
        return *this;
    }

    void InputBinaryArchive::SeekStream(StreamPosition position)
    {
        file.SeekStream(position);
    }

    InputBinaryArchive::StreamPosition InputBinaryArchive::TellStream()
    {
        return file.TellStream();
    }

    void InputBinaryArchive::InitialSetup()
    {
        const auto size = clientSignature.size();
        Signature loadedClientSignature(size, '\000');
        ContainerSize address = 0;
        while (address < size)
        {
            ReadImpl(loadedClientSignature[address]);
            ++address;
        }

        if (loadedClientSignature != clientSignature)
            throw InvalidSignature();

        ReadImpl(inscriptionVersion);
        ReadImpl(clientVersion);
        this->clientSignature = loadedClientSignature;
        postHeaderPosition = TellStream();
    }
}