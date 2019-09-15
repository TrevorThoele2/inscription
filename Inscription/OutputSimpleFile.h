#pragma once

#include <fstream>

#include "SimpleFile.h"

namespace Inscription
{
    class OutputSimpleFile : public SimpleFile<std::ofstream>
    {
    public:
        explicit OutputSimpleFile(const FilePath& path, bool append = false);

        template<class T>
        OutputSimpleFile& operator<<(T& var);

        void SeekP(StreamT::pos_type position);
    };

    template<class T>
    OutputSimpleFile& OutputSimpleFile::operator<<(T& var)
    {
        stream.write(reinterpret_cast<const char*>(&var), sizeof(var));
        return *this;
    }
}