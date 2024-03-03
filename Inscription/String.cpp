
#include "String.h"
#include "Tracking.h"
#include "ContainerSize.h"
#include "Scribe.h"
#include "Const.h"

namespace Inscription
{
    void Save(Scribe &scribe, const std::string &obj)
    {
        TrackingChangerStack stack(scribe, false);

        ContainerSize size(obj.size());
        scribe.Save(size);
        for (auto &loop : obj)
            scribe.Save(RemoveConst(loop));
    }

    void Load(Scribe &scribe, std::string &obj)
    {
        TrackingChangerStack stack(scribe, false);

        ContainerSize size;
        scribe.Load(size);

        obj.resize(size.Get());
        ContainerSize::ValueT address = 0;
        while (address < size.Get())
        {
            scribe.Load(obj[address]);
            ++address;
        }
    }

    void Serialize(Scribe &scribe, std::string &obj)
    {
        (scribe.IsOutput()) ? Save(scribe, obj) : Load(scribe, obj);
    }
}