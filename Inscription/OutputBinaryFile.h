#pragma once

#include <fstream>

#include "Stream.h"
#include "Buffer.h"

namespace Inscription
{
    class OutputBinaryFile : public Stream<std::ofstream>
    {
    public:
        explicit OutputBinaryFile(const FilePath& path);
        OutputBinaryFile(OutputBinaryFile&& arg) noexcept;

        OutputBinaryFile& operator=(OutputBinaryFile&& arg) noexcept;

        void WriteData(const Buffer& buffer);
        template<class T>
        void WriteData(T var);

        void SeekStreamFromCurrent(StreamPosition offset);
        void SeekStreamFromBegin(StreamPosition offset = 0);
        void SeekStreamFromEnd(StreamPosition offset = 0);
        [[nodiscard]] StreamPosition TellStream();
    };

    template<class T>
    void OutputBinaryFile::WriteData(T var)
    {
        stream.write(reinterpret_cast<const char*>(&var), sizeof(var));
        if (FailedStream())
            throw FileEncounteredError();
    }
}