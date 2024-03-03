#include "OutputBinaryScribe.h"

#include "ContainerSize.h"
#include "Const.h"

namespace Inscription
{
    OutputBinaryScribe::OutputBinaryScribe(const Path& path, const Signature& signature, Version version) :
        BinaryScribe(Direction::OUTPUT, signature, version), file(path)
    {
        for (auto& loop : signature)
            Save(RemoveConst(loop));

        (*this)(version);
        postHeaderPosition = TellStream();
    }

    OutputBinaryScribe::OutputBinaryScribe(OutputBinaryScribe&& arg) :
        BinaryScribe(std::move(arg)), file(std::move(arg.file))
    {}

    OutputBinaryScribe& OutputBinaryScribe::operator=(OutputBinaryScribe&& arg)
    {
        BinaryScribe::operator=(std::move(arg));
        file = std::move(arg.file);
        return *this;
    }

    void OutputBinaryScribe::SeekStream(StreamPosition position)
    {
        file.SeekStream(position);
    }

    OutputBinaryScribe::StreamPosition OutputBinaryScribe::TellStream()
    {
        return file.TellStream();
    }
}