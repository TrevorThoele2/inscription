#pragma once

#include "Stream.h"

namespace Inscription
{
    template<class T>
    class SimpleFile : public Stream<T>
    {
    public:
        using typename Stream<T>::Mode;
        using typename Stream<T>::StreamT;
        using typename Stream<T>::StreamPosition;
        using typename Stream<T>::OffsetPosition;
        using typename Stream<T>::SizeT;
    public:
        virtual ~SimpleFile() = 0;

        [[nodiscard]] bool IsAtEndOfFile();
    protected:
        // This constructor will not open the stream automatically
        // Be sure to ChangeMode into something that actually makes sense
        // Call reopen when you're ready to open the stream
        explicit SimpleFile(const FilePath& path);
        SimpleFile(const FilePath& path, Mode mode);
        SimpleFile(SimpleFile&& arg) noexcept;

        SimpleFile& operator=(SimpleFile&& arg) noexcept;
    };

    template<class T>
    SimpleFile<T>::SimpleFile(const FilePath& path) : Stream<T>(path)
    {}

    template<class T>
    SimpleFile<T>::SimpleFile(const FilePath& path, Mode mode) : Stream<T>(path, mode)
    {}

    template<class T>
    SimpleFile<T>::SimpleFile(SimpleFile&& arg) noexcept : Stream<T>(std::move(arg))
    {}

    template<class T>
    SimpleFile<T>& SimpleFile<T>::operator=(SimpleFile&& arg) noexcept
    {
        Stream<T>::operator=(std::move(arg));
        return *this;
    }

    template<class T>
    SimpleFile<T>::~SimpleFile() = default;

    template<class T>
    bool SimpleFile<T>::IsAtEndOfFile()
    {
        return this->stream.peek() == -1;
    }
}