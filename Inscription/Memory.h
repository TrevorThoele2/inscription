#pragma once

#include <memory>

#include "BinaryScribe.h"

namespace Inscription
{
    template<class T, class Delete>
    void Save(OutputBinaryScribe& scribe, const std::unique_ptr<T, Delete>& obj)
    {
        scribe.SaveOwningPointer(obj.get());
    }

    template<class T, class Delete>
    void Load(InputBinaryScribe& scribe, std::unique_ptr<T, Delete>& obj)
    {
        T* ptr = nullptr;
        scribe.LoadOwningPointer(ptr);
        obj.reset(ptr);
    }

    template<class T, class Delete>
    void Serialize(BinaryScribe& scribe, std::unique_ptr<T, Delete>& obj)
    {
        (scribe.IsOutput()) ? Save(*scribe.AsOutput(), obj) : Load(*scribe.AsInput(), obj);
    }
}