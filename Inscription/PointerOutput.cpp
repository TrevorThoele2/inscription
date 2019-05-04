
#include "PointerOutput.h"

#include "Scribe.h"

#include "PointerSpecialIDs.h"

namespace Inscription
{
    void PointerOutput::PolymorphicEntry::SaveID(Scribe& scribe)
    {
        scribe.Save(id);
    }

    void PointerOutput::PolymorphicEntry::SaveClassName(Scribe& scribe)
    {
        scribe.Save(RemoveConst(className));
    }

    void PointerOutput::PolymorphicEntry::SaveObject(const void* obj, Scribe& scribe, TrackerGroup& trackers, bool owns)
    {
        _saveObject(obj, scribe, trackers, owns);
    }

    TrackerID PointerOutput::PolymorphicEntry::FindFromTracker(const void* obj, TrackerGroup& trackers)
    {
        return _findFromTracker(obj, trackers);
    }

    TrackerID PointerOutput::PolymorphicEntry::Track(const void* obj, TrackerGroup& trackers)
    {
        return _track(obj, trackers);
    }

    void PointerOutput::AddEntry(const PolymorphicEntry& add, const std::type_index& type)
    {
        list.push_back(add);
        types.emplace(type, --list.end());
    }

    void PointerOutput::HandleNullptr(Scribe& scribe)
    {
        scribe.Save(PolymorphicID(PointerSpecialIDs::NULLPTR));
    }
}