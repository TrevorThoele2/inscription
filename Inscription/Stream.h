#pragma once

#include <fstream>

#include "FilePath.h"

#include "UnopenableFile.h"
#include "FileEncounteredError.h"

namespace Inscription
{
    template<class T>
    class Stream
    {
    public:
        using Mode = std::ios_base::openmode;
        using StreamT = T;
        using StreamPosition = std::streampos;
        using SizeT = StreamPosition;
    public:
        virtual ~Stream() = 0;

        void Open();
        void Close();
        bool IsOpen() const;

        void ChangeMode(Mode set);
    protected:
        StreamT stream;
    protected:
        // This constructor will not open the stream automatically
        // Be sure to ChangeMode into something that actually makes sense
        // Call reopen when you're ready to open the stream
        Stream(const FilePath& path);
        Stream(const FilePath& path, Mode mode);
        Stream(Stream&& arg);

        Stream& operator=(Stream&& arg);
    protected:
        bool FailedStream() const;
    private:
        FilePath path;
        Mode mode;
        bool isOpen;
    private:
        Stream() = delete;
        Stream(const Stream& arg) = delete;
    };

    template<class T>
    Stream<T>::~Stream()
    {}

    template<class T>
    void Stream<T>::Open()
    {
        if (isOpen)
            return;

        stream.open(path.c_str(), mode);
        if (FailedStream())
            throw UnopenableFile(path);

        isOpen = true;
    }

    template<class T>
    void Stream<T>::Close()
    {
        if (!isOpen)
            return;

        stream.close();
        if (FailedStream())
            throw FileEncounteredError();

        isOpen = false;
    }

    template<class T>
    bool Stream<T>::IsOpen() const
    {
        return isOpen;
    }

    template<class T>
    void Stream<T>::ChangeMode(Mode set)
    {
        mode = set;
        if (!isOpen)
            return;

        Close();
        Open();
    }

    template<class T>
    Stream<T>::Stream(const FilePath& path) : path(path), isOpen(false)
    {}

    template<class T>
    Stream<T>::Stream(const FilePath& path, Mode mode) : path(path), mode(mode), isOpen(false)
    {
        Open();
    }

    template<class T>
    Stream<T>::Stream(Stream&& arg) :
        stream(std::move(arg.stream)), path(std::move(arg.path)),
        mode(std::move(arg.mode)), isOpen(std::move(arg.isOpen))
    {}

    template<class T>
    Stream<T>& Stream<T>::operator=(Stream&& arg)
    {
        path = std::move(arg.path);
        mode = std::move(arg.mode);
        isOpen = std::move(arg.isOpen);
        return *this;
    }

    template<class T>
    bool Stream<T>::FailedStream() const
    {
        return stream.bad();
    }
}