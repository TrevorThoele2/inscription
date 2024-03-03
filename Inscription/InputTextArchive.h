#pragma once

#include "TextArchive.h"
#include "InputTextFile.h"

#include "InvalidArchiveDirection.h"

namespace Inscription
{
    class InputTextArchive : public TextArchive
    {
    public:
        InputTextArchive(const FilePath& path);
        InputTextArchive(InputTextArchive&& arg);

        InputTextArchive& operator=(InputTextArchive&& arg);

        InputTextArchive& ReadLine(std::string& arg);
        InputTextArchive& ReadLine(std::string& arg, char delimiter);
        InputTextArchive& ReadSize(std::string& arg, size_t size);
    protected:
        inline void ReadImpl(std::string& arg) { ReadFromFile(arg); }
        inline void ReadImpl(char& arg) { ReadFromFile(arg); }
        inline void ReadLineImpl(std::string& arg) { ReadLineFromFile(arg); }
        inline void ReadLineImpl(std::string& arg, char delimiter) { ReadLineFromFile(arg, delimiter); }
        inline void ReadSizeImpl(std::string& arg, size_t size) { ReadSizeFromFile(arg, size); }
    private:
        InputTextFile file;
    private:
        template<class T>
        void ReadFromFile(T& arg);

        void ReadLineFromFile(std::string& arg);
        void ReadLineFromFile(std::string& arg, char delimiter);
        void ReadSizeFromFile(std::string& arg, size_t size);
    };

    template<class T>
    void InputTextArchive::ReadFromFile(T& arg)
    {
        file.ReadData(arg);
    }
}