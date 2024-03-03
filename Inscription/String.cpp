#include "String.h"

#include "OutputBinaryScribe.h"
#include "InputBinaryScribe.h"

#include "ContainerSize.h"
#include "ScopedTrackingChanger.h"
#include "Const.h"

namespace Inscription
{
    void Save(OutputBinaryScribe& scribe, const std::string& obj)
    {
        ScopedTrackingChanger tracking(scribe, false);

        ContainerSize size(obj.size());
        scribe.Save(size);
        for (auto &loop : obj)
            scribe.Save(RemoveConst(loop));
    }

    void Load(InputBinaryScribe& scribe, std::string& obj)
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
        (scribe.IsOutput()) ? Save(*scribe.AsOutput(), obj) : Load(*scribe.AsInput(), obj);
    }
}