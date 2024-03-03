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

    void InputBinaryArchive::SeekStream(StreamPosition position)
    {
        file.SeekStream(position);
    }

    InputBinaryArchive::StreamPosition InputBinaryArchive::TellStream()
    {
        return file.TellStream();
    }
}