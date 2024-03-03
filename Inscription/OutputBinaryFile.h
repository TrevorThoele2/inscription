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
        void WriteData(const T var);

        void SeekStream(StreamPosition position);
        StreamPosition TellStream();
    };

    template<class T>
    void OutputBinaryFile::WriteData(const T var)
    {
        stream.write(reinterpret_cast<const char*>(&var), sizeof(var));
        if (FailedStream())
            throw FileEncounteredError();
    }
}