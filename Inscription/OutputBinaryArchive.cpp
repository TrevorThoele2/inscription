#include "OutputBinaryArchive.h"

namespace Inscription
{
    OutputBinaryArchive::OutputBinaryArchive(const FilePath& path) :
        BinaryArchive(Direction::Output),
        file(path)
    {}

    OutputBinaryArchive::OutputBinaryArchive(
        const FilePath& path, const TypeRegistrationContext& typeRegistrationContext) :
        BinaryArchive(Direction::Output, typeRegistrationContext),
        file(path)
    {}

    OutputBinaryArchive::OutputBinaryArchive(OutputBinaryArchive&& arg) noexcept :
        BinaryArchive(std::move(arg)), file(std::move(arg.file))
    {}

    OutputBinaryArchive& OutputBinaryArchive::operator=(OutputBinaryArchive&& arg) noexcept
    {
        BinaryArchive::operator=(std::move(arg));
        file = std::move(arg.file);
        return *this;
    }

    void OutputBinaryArchive::SeekStreamFromCurrent(StreamPosition position)
    {
        file.SeekStreamFromCurrent(position);
    }

    void OutputBinaryArchive::SeekStreamFromBegin(StreamPosition offset)
    {
        file.SeekStreamFromBegin(offset);
    }

    void OutputBinaryArchive::SeekStreamFromEnd(StreamPosition offset)
    {
        file.SeekStreamFromEnd(offset);
    }

    OutputBinaryArchive::StreamPosition OutputBinaryArchive::CurrentStreamPosition()
    {
        return file.TellStream();
    }
}