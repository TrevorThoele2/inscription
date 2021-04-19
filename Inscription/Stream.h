#pragma once

#include <functional>
#include "StreamPosition.h"
#include "FilePath.h"

#include "FileEncounteredError.h"

namespace Inscription
{
    template<class T>
    class Stream
    {
    public:
        using Mode = std::ios_base::openmode;
        using StreamT = T;
        using StreamPosition = StreamPosition;
        using OffsetPosition = StreamPosition;
        using SizeT = std::streamsize;
    public:
        Stream() = delete;
        Stream(const FilePath& path, Mode mode);
        Stream(const Stream& arg) = delete;
        Stream(Stream&& arg) noexcept;

        Stream& operator=(Stream&& arg) noexcept;
        
        void Write(const char* bytes, std::streamsize count);
        void WriteData(const std::string& string);
        void WriteData(char character);

        void Read(char* bytes, std::streamsize count);
        void ReadData(std::string& string);
        void ReadData(char& character);
        [[nodiscard]] std::string GetLine() const;
        [[nodiscard]] std::string GetLine(char delimiter) const;

        void Ignore(SizeT count = 1, int delimiter = EOF);
        void Clear(std::ios::iostate state = std::ios::goodbit);

        void SeekP(StreamPosition position);
        void SeekP(StreamPosition position, std::ios::seekdir direction);
        [[nodiscard]] std::ios::pos_type TellP() const;

        void SeekG(StreamPosition position);
        void SeekG(StreamPosition position, std::ios::seekdir direction);
        [[nodiscard]] std::ios::pos_type TellG() const;

        [[nodiscard]] SizeT GCount() const;
    private:
        StreamT stream;

        FilePath path;
        Mode mode;
    private:
    private:
        void Open();

        void SetupExceptions();

    };
    
    template<class T>
    Stream<T>::Stream(const FilePath& path, Mode mode) : path(path), mode(mode)
    {
        SetupExceptions();
        Open();
    }

    template<class T>
    Stream<T>::Stream(Stream&& arg) noexcept :
        stream(std::move(arg.stream)),
        path(std::move(arg.path)),
        mode(std::move(arg.mode))
    {}

    template<class T>
    Stream<T>& Stream<T>::operator=(Stream&& arg) noexcept
    {
        path = std::move(arg.path);
        mode = std::move(arg.mode);
        return *this;
    }
    
    template<class T>
    void Stream<T>::Write(const char* bytes, std::streamsize count)
    {
        SanitizeStreamFailure([this, bytes, count]() { stream.write(bytes, count); });
    }

    template<class T>
    void Stream<T>::WriteData(const std::string& string)
    {
        SanitizeStreamFailure([this, string]()
            {
                if (width > 0)
                {
                    const auto previousFill = stream.fill();
                    const auto previousWidth = stream.width();

                    stream.fill(fillCharacter);
                    stream.width(width);

                    stream << string;

                    stream.fill(previousFill);
                    stream.width(previousWidth);
                }
                else
                    stream << string;
            });
    }

    template<class T>
    void Stream<T>::WriteData(char character)
    {
        SanitizeStreamFailure([this, character]() { stream << character; });
    }

    template<class T>
    void Stream<T>::Read(char* bytes, std::streamsize count)
    {
        SanitizeStreamFailure([this, bytes, count]() { stream.read(bytes, count); });
    }

    template<class T>
    void Stream<T>::ReadData(std::string& string)
    {
        SanitizeStreamFailure([this, &string]() { stream >> string; });
    }

    template<class T>
    void Stream<T>::ReadData(char& character)
    {
        SanitizeStreamFailure([this, &character]() { stream >> character; });
    }

    template<class T>
    std::string Stream<T>::GetLine() const
    {
        return SanitizeStreamFailure([this]()
            {
                std::string string;
                std::getline(stream, string);
                return string;
            });
    }

    template<class T>
    std::string Stream<T>::GetLine(char delimiter) const
    {
        return SanitizeStreamFailure([this, delimiter]()
            {
                std::string string;
                std::getline(stream, string, delimiter);
                return string;
            });
    }

    template<class T>
    void Stream<T>::Ignore(SizeT count, int delimiter)
    {
        SanitizeStreamFailure([this, count, delimiter]() { stream.ignore(count, delimiter); });
    }

    template<class T>
    void Stream<T>::Clear(std::ios::iostate state)
    {
        SanitizeStreamFailure([this, state]() { stream.clear(state); });
    }

    template<class T>
    void Stream<T>::SeekP(StreamPosition position)
    {
        SanitizeStreamFailure([this, position]() { stream.seekp(position); });
    }

    template<class T>
    void Stream<T>::SeekP(StreamPosition position, std::ios::seekdir direction)
    {
        SanitizeStreamFailure([this, position, direction]() { stream.seekp(position, direction); });
    }

    template<class T>
    std::ios::pos_type Stream<T>::TellP() const
    {
        return SanitizeStreamFailure([this]() { return stream.tellp(); });
    }

    template<class T>
    void Stream<T>::SeekG(StreamPosition position)
    {
        SanitizeStreamFailure([this, position]() { stream.seekg(position); });
    }

    template<class T>
    void Stream<T>::SeekG(StreamPosition position, std::ios::seekdir direction)
    {
        SanitizeStreamFailure([this, position, direction]() { stream.seekg(position, direction); });
    }

    template<class T>
    std::ios::pos_type Stream<T>::TellG() const
    {
        return SanitizeStreamFailure([this]() { return stream.tellg(); });
    }

    template<class T>
    auto Stream<T>::GCount() const -> SizeT
    {
        return SanitizeStreamFailure([this]() { return stream.gcount(); });
    }

    template<class T>
    void Stream<T>::Open()
    {
        SanitizeStreamFailure([this]() { stream.open(); });
    }

    template<class T>
    void Stream<T>::SetupExceptions()
    {
        stream.exceptions(std::ios::failbit | std::ios::badbit);
    }
}