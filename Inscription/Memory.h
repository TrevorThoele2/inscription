#pragma once

#include <memory>

namespace inscription
{
    class Scribe;
    template<class T, class Delete>
    void Save(Scribe &scribe, const std::unique_ptr<T, Delete> &obj)
    {
        scribe.Save(obj.get());
    }

    template<class T, class Delete>
    void Load(Scribe &scribe, std::unique_ptr<T, Delete> &obj)
    {
        T *ptr = nullptr;
        scribe.Load(ptr);
        obj.reset(ptr);
    }

    template<class T, class Delete>
    void Serialize(Scribe &scribe, std::unique_ptr<T, Delete> &obj)
    {
        (scribe.IsOutput()) ? Save(scribe, obj) : Load(scribe, obj);
    }
}