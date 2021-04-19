#pragma once

#include <functional>
#include "FilePath.h"
#include "FileEncounteredError.h"

namespace Inscription::File
{
    inline std::string CurrentError()
    {
        std::string message;
        message.resize(1024);
        strerror_s(message.data(), message.size(), errno);
        return message;
    }

    inline void SanitizeStreamFailure(const std::function<void()>& function, const Path& path)
    {
        try
        {
            function();
        }
        catch (std::system_error&)
        {
            throw FileEncounteredError(CurrentError(), path);
        }
    }

    template<class ReturnT>
    ReturnT SanitizeStreamFailure(const std::function<ReturnT()>& function, const Path& path)
    {
        try
        {
            return function();
        }
        catch (std::system_error&)
        {
            throw FileEncounteredError(CurrentError(), path);
        }
    }
}