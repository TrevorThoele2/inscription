#pragma once

#include <string>

#include "Archive.h"
#include "Direction.h"

namespace Inscription
{
    class TextArchive : public Archive
    {
    public:
        virtual ~TextArchive() = 0;
    public:
        template<class T>
        inline TextArchive& operator()(T&& arg);
        template<class T>
        inline TextArchive& Save(T&& arg);
        template<class T>
        inline TextArchive& Load(T&& arg);

        TextArchive& LoadLine(std::string& arg);
        TextArchive& LoadLine(std::string& arg, char delimiter);
        TextArchive& LoadSize(std::string& arg, size_t size);
    public:
        bool IsOutput() const;
        bool IsInput() const;
    protected:
        TextArchive(Direction direction);
    protected:
        virtual void WriteImpl(const std::string& arg) = 0;
        virtual void WriteImpl(const char arg) = 0;

        virtual void ReadImpl(std::string& arg) = 0;
        virtual void ReadImpl(char& arg) = 0;
        virtual void ReadLineImpl(std::string& arg) = 0;
        virtual void ReadLineImpl(std::string& arg, char delimiter) = 0;
        virtual void ReadSizeImpl(std::string& arg, size_t size) = 0;
    private:
        const Direction direction;
    };

    template<class T>
    TextArchive& TextArchive::operator()(T&& arg)
    {
        switch (direction)
        {
        case Direction::OUTPUT:
            Save(std::move(arg));
            break;
        case Direction::INPUT:
            Load(std::move(arg));
            break;
        }

        return *this;
    }

    template<class T>
    TextArchive& TextArchive::Save(T&& arg)
    {
        WriteImpl(std::move(arg));
        return *this;
    }

    template<class T>
    TextArchive& TextArchive::Load(T&& arg)
    {
        ReadImpl(arg);
        return *this;
    }
}