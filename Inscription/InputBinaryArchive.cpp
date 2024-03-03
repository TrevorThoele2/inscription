#include "InputBinaryArchive.h"

#include "ContainerSize.h"

#include "InvalidSignature.h"

namespace Inscription
{
    InputBinaryArchive::InputBinaryArchive(const Path& path, const Signature& signature) :
        BinaryArchive(Direction::INPUT, signature, 0), file(path)
    {
        InitialSetup();
    }

    InputBinaryArchive::InputBinaryArchive(const Path& path, const Signature& signature, const TypeRegistrationContext& typeRegistrationContext) :
        BinaryArchive(Direction::INPUT, signature, 0, typeRegistrationContext), file(path)
    {
        InitialSetup();
    }

    InputBinaryArchive::InputBinaryArchive(InputBinaryArchive&& arg) :
        BinaryArchive(std::move(arg)), file(std::move(arg.file))
    {}

    InputBinaryArchive& InputBinaryArchive::operator=(InputBinaryArchive&& arg)
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
        auto size = signature.size();
        Signature loadSignature(size, '\000');
        ContainerSize address = 0;
        while (address < size)
        {
            ReadImpl(loadSignature[address]);
            ++address;
        }

        if (loadSignature != signature)
            throw InvalidSignature();

        ReadImpl(version);
        this->signature = loadSignature;
        postHeaderPosition = TellStream();
    }
}