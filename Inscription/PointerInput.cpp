
#include "PointerInput.h"

#include "Scribe.h"

#include "PointerSpecialIDs.h"

namespace Inscription
{
    void PointerInput::PolymorphicEntry::LoadObject(void*& obj, Scribe &scribe, TrackerGroup& trackers, bool owns)
    {
        _loadObject(obj, scribe, trackers, owns);
    }

    void PointerInput::AddEntry(const PolymorphicEntry& add)
    {
        list.push_back(add);
        classNameToPolymorphicEntry.emplace(add.className, --list.end());
    }

    bool PointerInput::ShouldHandleNullptr(PolymorphicID id) const
    {
        return id == PointerSpecialIDs::NULLPTR;
    }

    bool PointerInput::ShouldHandleNonpolymorphically(PolymorphicID id) const
    {
        return id == PointerSpecialIDs::HANDLE_NONPOLYMORPHICALLY;
    }

    PolymorphicID PointerInput::LoadPolymorphicID(Scribe& scribe)
    {
        PolymorphicID loaded = PointerSpecialIDs::NULLPTR;
        scribe.Load(loaded);
        return loaded;
    }

    TrackerID PointerInput::LoadTrackerID(Scribe& scribe)
    {
        TrackerID loaded = 0;
        scribe.Load(loaded);
        return loaded;
    }

    PointerInput::PolymorphicIdToClassNames::iterator PointerInput::EmplaceClassName(const ClassName& className)
    {
        return polymorphicIdToClassNames.emplace(nextID++, className).first;
    }

    ClassName* PointerInput::FindClassName(PolymorphicID polymorphicId)
    {
        auto found = polymorphicIdToClassNames.find(polymorphicId);
        if (found == polymorphicIdToClassNames.end())
            return nullptr;

        return &found->second;
    }

    PointerInput::PolymorphicEntryList::iterator PointerInput::FindPolymorphicEntry(const ClassName& className)
    {
        auto found = classNameToPolymorphicEntry.find(className);
        if (found == classNameToPolymorphicEntry.end())
            return list.end();

        return found->second;
    }
}