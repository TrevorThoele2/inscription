#pragma once

#include <unordered_map>

#include "TrackingID.h"
#include "ObjectTrackingMapEntry.h"

namespace Inscription
{
    class ObjectTrackingMap
    {
    public:
        typedef TrackingID ID;
        typedef ObjectTrackingMapEntry Entry;
    public:
        ObjectTrackingMap() = default;
        ObjectTrackingMap(const ObjectTrackingMap& arg) = default;
        ObjectTrackingMap(ObjectTrackingMap&& arg);

        ObjectTrackingMap& operator=(const ObjectTrackingMap& arg) = default;
        ObjectTrackingMap& operator=(ObjectTrackingMap&& arg);

        ID Add(void* add, bool section);
        ID Add(void* add, bool section, ID id);

        void ReplaceObject(void* here, void* newObj, bool section);

        Entry* Find(const void* find);
        const Entry* Find(const void* find) const;
        Entry* Find(ID id);
        const Entry* Find(ID id) const;

        void Clear();

        void ClearSection(bool deleteEntries);
    private:
        typedef std::unordered_map<ID, Entry> Map;
        typedef typename Map::iterator iterator;
        typedef typename Map::const_iterator const_iterator;

        typedef std::unordered_map<const void*, iterator> Checker;
        typedef std::unordered_map<const void*, iterator> Section;
    private:
        Map map;
        Checker checker;

        Section section;
    private:
        bool IsTracking(const void* obj) const;
    private:
        ID NextID() const;
    };
}