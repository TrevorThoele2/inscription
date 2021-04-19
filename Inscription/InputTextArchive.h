#pragma once

#include "TextArchive.h"
#include "InputTextFile.h"

namespace Inscription::Archive
{
    class InputText : public Text
    {
    public:
        explicit InputText(const File::Path& path);
        InputText(const InputText& arg) = delete;
        InputText(InputText&& arg) noexcept;
        
        InputText& operator=(const InputText& arg) = delete;
        InputText& operator=(InputText&& arg) noexcept;

        InputText& ReadLine(std::string& arg);
        InputText& ReadLine(std::string& arg, char delimiter);
        InputText& ReadSize(std::string& arg, size_t size = std::numeric_limits<size_t>::max());
    protected:
        void ReadImpl(std::string& arg) { ReadFromFile(arg); }
        void ReadImpl(char& arg) { ReadFromFile(arg); }
        void ReadLineImpl(std::string& arg) { ReadLineFromFile(arg); }
        void ReadLineImpl(std::string& arg, char delimiter) { ReadLineFromFile(arg, delimiter); }
        void ReadSizeImpl(std::string& arg, size_t size) { ReadSizeFromFile(arg, size); }
    private:
        File::InputText file;
    private:
        template<class T>
        void ReadFromFile(T& arg);

        void ReadLineFromFile(std::string& arg);
        void ReadLineFromFile(std::string& arg, char delimiter);
        void ReadSizeFromFile(std::string& arg, size_t size);
    };

    template<class T>
    void InputText::ReadFromFile(T& arg)
    {
        file.ReadData(arg);
    }
}