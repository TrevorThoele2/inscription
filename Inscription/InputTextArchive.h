#pragma once

#include "TextArchive.h"
#include "InputTextFile.h"

namespace Inscription
{
    class InputTextArchive : public TextArchive
    {
    public:
        explicit InputTextArchive(const FilePath& path);
        InputTextArchive(const InputTextArchive& arg) = delete;
        InputTextArchive(InputTextArchive&& arg) noexcept;

        ~InputTextArchive() = default;

        InputTextArchive& operator=(const InputTextArchive& arg) = delete;
        InputTextArchive& operator=(InputTextArchive&& arg) noexcept;

        InputTextArchive& ReadLine(std::string& arg);
        InputTextArchive& ReadLine(std::string& arg, char delimiter);
        InputTextArchive& ReadSize(std::string& arg, size_t size = std::numeric_limits<size_t>::max());
    protected:
        void ReadImpl(std::string& arg) { ReadFromFile(arg); }
        void ReadImpl(char& arg) { ReadFromFile(arg); }
        void ReadLineImpl(std::string& arg) { ReadLineFromFile(arg); }
        void ReadLineImpl(std::string& arg, char delimiter) { ReadLineFromFile(arg, delimiter); }
        void ReadSizeImpl(std::string& arg, size_t size) { ReadSizeFromFile(arg, size); }
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