#pragma once

#include <fstream>

#include "Stream.h"
#include "Buffer.h"

namespace Inscription
{
    class InputBinaryFile : public Stream<std::ifstream>
    {
    public:
        explicit InputBinaryFile(const FilePath& path);
        InputBinaryFile(InputBinaryFile&& arg) noexcept;

        InputBinaryFile& operator=(InputBinaryFile&& arg) noexcept;

        void ReadData(Buffer& buffer, size_t size);
        template<class T>
        void ReadData(T& var);

        void SeekStream(StreamPosition offset);
        void SeekStreamFromBegin(StreamPosition offset = 0);
        void SeekStreamFromEnd(StreamPosition offset = 0);
        [[nodiscard]] StreamPosition TellStream();
        [[nodiscard]] SizeT Size();
    };

    template<class T>
    void InputBinaryFile::ReadData(T& var)
    {
        stream.read(reinterpret_cast<char*>(&var), sizeof(var));
        if (stream.bad())
            throw FileEncounteredError();
    }
}