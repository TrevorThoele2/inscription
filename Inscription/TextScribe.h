#pragma once

#include <string>

#include "Scribe.h"
#include "Direction.h"

namespace Inscription
{
    class TextScribe : public Scribe
    {
    public:
        virtual ~TextScribe() = 0;
    public:
        template<class T>
        inline TextScribe& operator()(T&& arg);
        template<class T>
        inline TextScribe& Save(T&& arg);
        template<class T>
        inline TextScribe& Load(T&& arg);

        TextScribe& LoadLine(std::string& arg);
        TextScribe& LoadLine(std::string& arg, char delimiter);
        TextScribe& LoadSize(std::string& arg, size_t size);
    public:
        bool IsOutput() const;
        bool IsInput() const;
    protected:
        TextScribe(Direction direction);
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
    TextScribe& TextScribe::operator()(T&& arg)
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
    TextScribe& TextScribe::Save(T&& arg)
    {
        WriteImpl(std::move(arg));
        return *this;
    }

    template<class T>
    TextScribe& TextScribe::Load(T&& arg)
    {
        ReadImpl(arg);
        return *this;
    }
}