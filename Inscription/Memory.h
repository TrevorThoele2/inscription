#pragma once

#include <memory>

namespace Inscription
{
    class Scribe;
    template<class T, class Delete>
    void Save(Scribe &scribe, const std::unique_ptr<T, Delete> &obj)
    {
        scribe.SaveOwningPointer(obj.get());
    }

    template<class T, class Delete>
    void Load(Scribe &scribe, std::unique_ptr<T, Delete> &obj)
    {
        T *ptr = nullptr;
        scribe.LoadOwningPointer(ptr);
        obj.reset(ptr);
    }

    template<class T, class Delete>
    void Serialize(Scribe &scribe, std::unique_ptr<T, Delete> &obj)
    {
        (scribe.IsOutput()) ? Save(scribe, obj) : Load(scribe, obj);
    }
}