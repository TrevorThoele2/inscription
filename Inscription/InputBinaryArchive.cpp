#include "InputBinaryArchive.h"

namespace Inscription::Archive
{
    InputBinary::InputBinary(const File::Path& path) :
        Binary(Direction::Input),
        file(path)
    {}

    InputBinary::InputBinary(
        const File::Path& path, const TypeRegistrationContext& typeRegistrationContext)
        :
        Binary(Direction::Input, typeRegistrationContext),
        file(path)
    {}

    InputBinary::InputBinary(InputBinary&& arg) noexcept :
        Binary(std::move(arg)), file(std::move(arg.file))
    {}

    InputBinary& InputBinary::operator=(InputBinary&& arg) noexcept
    {
        Binary::operator=(std::move(arg));
        file = std::move(arg.file);
        return *this;
    }

    void InputBinary::Seek(File::Position offset)
    {
        file.Seek(offset);
    }

    void InputBinary::SeekFromBeginning(File::Position offset)
    {
        file.SeekFromBeginning(offset);
    }

    void InputBinary::SeekFromEnd(File::Position offset)
    {
        file.SeekFromEnd(offset);
    }

    File::Position InputBinary::Position()
    {
        return file.Position();
    }

    File::Size InputBinary::Size()
    {
        return file.Size();
    }
}