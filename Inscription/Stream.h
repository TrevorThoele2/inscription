
#pragma once

#include <fstream>
#include "Path.h"

namespace inscription
{
    template<class T>
    class Stream
    {
    public:
        typedef inscription::Path Path;
        typedef std::ios_base::openmode Mode;
        typedef T StreamT;
        typedef typename StreamT::streampos StreamPos;
    private:
        Path path;
        Mode mode;

        Stream() = delete;
        Stream(const Stream &arg) = delete;
        void Open();
    protected:
        StreamT stream;
    public:
        Stream(const Path &path, Mode mode);
        virtual ~Stream() = 0;
        void Close();
        void ChangeMode(Mode set);
        void Reopen();
    };

    template<class T>
    void Stream<T>::Open()
    {
        stream.open(path.c_str(), mode);
    }

    template<class T>
    Stream<T>::Stream(const Path &path, Mode mode) : path(path), mode(mode)
    {
        Open();
    }

    template<class T>
    Stream<T>::~Stream()
    {}

    template<class T>
    void Stream<T>::Close()
    {
        stream.close();
    }

    template<class T>
    void Stream<T>::ChangeMode(Mode set)
    {
        mode = set;
        Close();
        Open();
    }

    template<class T>
    void Stream<T>::Reopen()
    {
        Open();
    }
}