#pragma once

namespace Inscription
{
    template<class BaseObject, class Object, class Archive>
    Archive& BaseScriven(Object& object, Archive& archive)
    {
        archive(static_cast<BaseObject&>(object));
        return archive;
    }
}