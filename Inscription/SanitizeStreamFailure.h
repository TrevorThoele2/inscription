#pragma once

#include <functional>
#include "FilePath.h"
#include "FileEncounteredError.h"

namespace Inscription::File
{
    inline void SanitizeStreamFailure(const std::function<void()>& function, const Path& path)
    {
        try
        {
            function();
        }
        catch (std::system_error& error)
        {
            throw FileEncounteredError(error.code().message(), path);
        }
    }

    template<class ReturnT>
    ReturnT SanitizeStreamFailure(const std::function<ReturnT()>& function, const Path& path)
    {
        try
        {
            return function();
        }
        catch (std::system_error& error)
        {
            throw FileEncounteredError(error.code().message(), path);
        }
    }
}