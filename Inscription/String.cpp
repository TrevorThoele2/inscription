#include "String.h"

#include "BinaryScribe.h"

#include "ContainerSize.h"
#include "ScopedTrackingChanger.h"
#include "Const.h"

namespace Inscription
{
    void Save(BinaryScribe& scribe, const std::string& obj)
    {
        ScopedTrackingChanger tracking(scribe, false);

        ContainerSize size(obj.size());
        scribe.Save(size);
        for (auto &loop : obj)
            scribe.Save(RemoveConst(loop));
    }

    void Load(BinaryScribe& scribe, std::string& obj)
    {
        ScopedTrackingChanger tracking(scribe, false);

        ContainerSize size;
        scribe.Load(size);

        obj.resize(size);
        ContainerSize address = 0;
        while (address < size)
        {
            scribe.Load(obj[address]);
            ++address;
        }
    }

    void Serialize(BinaryScribe& scribe, std::string& obj)
    {
        (scribe.IsOutput()) ? Save(scribe, obj) : Load(scribe, obj);
    }
}