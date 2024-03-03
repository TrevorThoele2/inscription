#include "PointerInput.h"

#include "BinaryScribe.h"

namespace Inscription
{
    void PointerInput::PolymorphicEntry::LoadObject(void*& obj, BinaryScribe& scribe, TrackerMap& trackers, bool owns)
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
        return id == PointerSpecialID::NULLPTR;
    }

    bool PointerInput::ShouldHandleNonpolymorphically(PolymorphicID id) const
    {
        return id == PointerSpecialID::HANDLE_NONPOLYMORPHICALLY;
    }

    PolymorphicID PointerInput::LoadPolymorphicID(BinaryScribe& scribe)
    {
        PolymorphicID loaded = PointerSpecialID::NULLPTR;
        scribe.Load(loaded);
        return loaded;
    }

    TrackingID PointerInput::LoadTrackingID(BinaryScribe& scribe)
    {
        TrackingID loaded = 0;
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