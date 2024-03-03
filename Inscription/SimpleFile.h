#pragma once

#include "Stream.h"

namespace Inscription
{
    template<class T>
    class SimpleFile : public Stream<T>
    {
    public:
        using typename Stream<T>::FilePath;
        using typename Stream<T>::Mode;
        using typename Stream<T>::StreamT;
        using typename Stream<T>::StreamPosition;
        using typename Stream<T>::SizeT;
    public:
        virtual ~SimpleFile() = 0;

        bool IsAtEndOfFile() const;
    protected:
        // This constructor will not open the stream automatically
        // Be sure to ChangeMode into something that actually makes sense
        // Call reopen when you're ready to open the stream
        SimpleFile(const FilePath& path);
        SimpleFile(const FilePath& path, Mode mode);
        SimpleFile(SimpleFile&& arg);

        SimpleFile& operator=(SimpleFile&& arg);
    };

    template<class T>
    SimpleFile<T>::SimpleFile(const FilePath& path) : Stream<T>(path)
    {}

    template<class T>
    SimpleFile<T>::SimpleFile(const FilePath& path, Mode mode) : Stream<T>(path, mode)
    {}

    template<class T>
    SimpleFile<T>::SimpleFile(SimpleFile&& arg) : Stream<T>(std::move(arg))
    {}

    template<class T>
    SimpleFile<T>& SimpleFile<T>::operator=(SimpleFile&& arg)
    {
        Stream<T>::operator=(std::move(arg));
        return *this;
    }

    template<class T>
    SimpleFile<T>::~SimpleFile()
    {}

    template<class T>
    bool SimpleFile<T>::IsAtEndOfFile() const
    {
        return this->stream.eof();
    }
}