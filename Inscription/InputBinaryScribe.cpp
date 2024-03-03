#include "InputBinaryScribe.h"

#include "ContainerSize.h"

#include "InvalidSignature.h"

namespace Inscription
{
    InputBinaryScribe::InputBinaryScribe(const Path& path, const Signature& signature) :
        BinaryScribe(Direction::INPUT, signature, 0), file(path)
    {
        auto size = signature.size();
        Signature loadSignature(size, '\000');
        ContainerSize address = 0;
        while (address < size)
        {
            Load(loadSignature[address]);
            ++address;
        }

        if (loadSignature != signature)
            throw InvalidSignature();

        Load(version);
        this->signature = loadSignature;
        postHeaderPosition = TellStream();
    }

    InputBinaryScribe::InputBinaryScribe(InputBinaryScribe&& arg) :
        BinaryScribe(std::move(arg)), file(std::move(arg.file))
    {}

    InputBinaryScribe& InputBinaryScribe::operator=(InputBinaryScribe&& arg)
    {
        BinaryScribe::operator=(std::move(arg));
        file = std::move(arg.file);
        return *this;
    }

    void InputBinaryScribe::SeekStream(StreamPosition position)
    {
        file.SeekStream(position);
    }

    InputBinaryScribe::StreamPosition InputBinaryScribe::TellStream()
    {
        return file.TellStream();
    }
}