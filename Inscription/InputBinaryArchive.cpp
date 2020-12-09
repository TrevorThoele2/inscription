#include "InputBinaryArchive.h"

#include "ContainerSize.h"

namespace Inscription
{
    InputBinaryArchive::InputBinaryArchive(const FilePath& path) :
        BinaryArchive(Direction::Input),
        file(path)
    {}

    InputBinaryArchive::InputBinaryArchive(
        const FilePath& path, const TypeRegistrationContext& typeRegistrationContext)
        :
        BinaryArchive(Direction::Input, typeRegistrationContext),
        file(path)
    {}

    InputBinaryArchive::InputBinaryArchive(InputBinaryArchive&& arg) noexcept :
        BinaryArchive(std::move(arg)), file(std::move(arg.file))
    {}

    InputBinaryArchive& InputBinaryArchive::operator=(InputBinaryArchive&& arg) noexcept
    {
        BinaryArchive::operator=(std::move(arg));
        file = std::move(arg.file);
        return *this;
    }

    void InputBinaryArchive::SeekStreamFromCurrent(StreamPosition offset)
    {
        file.SeekStream(offset);
    }

    void InputBinaryArchive::SeekStreamFromBegin(StreamPosition offset)
    {
        file.SeekStreamFromBegin(offset);
    }

    void InputBinaryArchive::SeekStreamFromEnd(StreamPosition offset)
    {
        file.SeekStreamFromEnd(offset);
    }

    InputBinaryArchive::StreamPosition InputBinaryArchive::CurrentStreamPosition()
    {
        return file.TellStream();
    }

    auto InputBinaryArchive::Size() -> SizeT
    {
        return file.Size();
    }
}