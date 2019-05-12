#include "PointerOutput.h"

#include "BinaryScribe.h"

#include "PointerSpecialID.h"

namespace Inscription
{
    void PointerOutput::PolymorphicEntry::SaveID(BinaryScribe& scribe)
    {
        scribe.Save(id);
    }

    void PointerOutput::PolymorphicEntry::SaveClassName(BinaryScribe& scribe)
    {
        scribe.Save(RemoveConst(className));
    }

    void PointerOutput::PolymorphicEntry::SaveObject(const void* obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns)
    {
        _saveObject(obj, scribe, trackers, owns);
    }

    TrackingID PointerOutput::PolymorphicEntry::FindFromTracker(const void* obj, TrackerMap& trackers)
    {
        return _findFromTracker(obj, trackers);
    }

    TrackingID PointerOutput::PolymorphicEntry::Track(const void* obj, TrackerMap& trackers)
    {
        return _track(obj, trackers);
    }

    void PointerOutput::AddEntry(const PolymorphicEntry& add, const std::type_index& type)
    {
        list.push_back(add);
        types.emplace(type, --list.end());
    }

    void PointerOutput::HandleNullptr(BinaryScribe& scribe)
    {
        scribe.Save(PolymorphicID(PointerSpecialID::NULLPTR));
    }
}