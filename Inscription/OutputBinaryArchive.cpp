#include "OutputBinaryArchive.h"

namespace Inscription::Archive
{
    OutputBinary::OutputBinary(const File::Path& path) :
        Binary(Direction::Output),
        file(path)
    {}

    OutputBinary::OutputBinary(
        const File::Path& path, const TypeRegistrationContext& typeRegistrationContext)
        :
        Binary(Direction::Output, typeRegistrationContext),
        file(path)
    {}

    OutputBinary::OutputBinary(OutputBinary&& arg) noexcept :
        Binary(std::move(arg)), file(std::move(arg.file))
    {}

    OutputBinary& OutputBinary::operator=(OutputBinary&& arg) noexcept
    {
        Binary::operator=(std::move(arg));
        file = std::move(arg.file);
        return *this;
    }

    void OutputBinary::Seek(File::Position position)
    {
        file.Seek(position);
    }

    void OutputBinary::SeekFromBeginning(File::Position offset)
    {
        file.SeekFromBeginning(offset);
    }

    void OutputBinary::SeekFromEnd(File::Position offset)
    {
        file.SeekFromEnd(offset);
    }

    File::Position OutputBinary::Position()
    {
        return file.Position();
    }
}