#include "InputBinaryArchive.h"

namespace Inscription::Archive
{
    InputBinary::InputBinary(File::InputBinary& file) : Binary(Direction::Input), source(&file)
    {}

    InputBinary::InputBinary(Buffer& buffer) : Binary(Direction::Input), source(&buffer)
    {}
    
    InputBinary::InputBinary(InputBinary&& arg) noexcept :
        Binary(std::move(arg)), source(std::move(arg.source))
    {}

    InputBinary& InputBinary::operator=(InputBinary&& arg) noexcept
    {
        Binary::operator=(std::move(arg));
        source = std::move(arg.source);
        return *this;
    }

    File::InputBinary* InputBinary::File() const
    {
        return std::holds_alternative<File::InputBinary*>(source) ? std::get<File::InputBinary*>(source) : nullptr;
    }
    
	void InputBinary::PullFromBuffer(Buffer& arg, Buffer& buffer)
    {
        const auto size = arg.value.size();
        if (size > 0)
        {
            memcpy(&arg.value[0], &buffer.value[0], size);
            buffer.value.erase(buffer.value.begin(), buffer.value.begin() + size);
        }
        else
            arg = {};
    }
}