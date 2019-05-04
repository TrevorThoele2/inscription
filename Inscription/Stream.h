
#pragma once

#include <fstream>
#include "Path.h"

namespace Inscription
{
    template<class T>
    class Stream
    {
    public:
        typedef ::Inscription::Path Path;
        typedef std::ios_base::openmode Mode;
        typedef T StreamT;
        typedef std::streampos StreamPosition;
        typedef StreamPosition SizeT;
    private:
        Path path;
        Mode mode;
        bool open;

        Stream() = delete;
        Stream(const Stream &arg) = delete;
        void Open();
    protected:
        StreamT stream;
        // This constructor will not open the stream automatically
        // Be sure to ChangeMode into something that actually makes sense
        // Call reopen when you're ready to open the stream
        Stream(const Path &path);
        Stream(const Path &path, Mode mode);
    public:
        virtual ~Stream() = 0;
        void Close();
        void ChangeMode(Mode set);
        void Reopen();
        bool IsOpen() const;
    };

    template<class T>
    void Stream<T>::Open()
    {
        if (open)
            return;

        stream.open(path.c_str(), mode);
        open = false;
    }

    template<class T>
    Stream<T>::Stream(const Path &path) : path(path), open(false)
    {}

    template<class T>
    Stream<T>::Stream(const Path &path, Mode mode) : path(path), mode(mode), open(false)
    {
        Open();
    }

    template<class T>
    Stream<T>::~Stream()
    {}

    template<class T>
    void Stream<T>::Close()
    {
        if (!open)
            return;

        stream.close();
        open = false;
    }

    template<class T>
    void Stream<T>::ChangeMode(Mode set)
    {
        mode = set;
        if (!open)
            return;

        Close();
        Open();
    }

    template<class T>
    void Stream<T>::Reopen()
    {
        Open();
    }

    template<class T>
    bool Stream<T>::IsOpen() const
    {
        return open;
    }
}