#pragma once

#include <memory>

namespace Inscription
{
    template<class ScribeT, class T, class Delete>
    void Save(ScribeT& scribe, const std::unique_ptr<T, Delete>& obj)
    {
        scribe.SaveOwningPointer(obj.get());
    }

    template<class ScribeT, class T, class Delete>
    void Load(ScribeT& scribe, std::unique_ptr<T, Delete>& obj)
    {
        T* ptr = nullptr;
        scribe.LoadOwningPointer(ptr);
        obj.reset(ptr);
    }

    template<class ScribeT, class T, class Delete>
    void Serialize(ScribeT& scribe, std::unique_ptr<T, Delete>& obj)
    {
        (scribe.IsOutput()) ? Save(*scribe.AsOutput(), obj) : Load(*scribe.AsInput(), obj);
    }
}