#pragma once

#include <unordered_map>

#include "TrackingID.h"
#include "TrackingEntry.h"
#include "TrackingLookaheadGroup.h"

namespace Inscription
{
    class Tracker
    {
    public:
        typedef TrackingID ID;
        typedef TrackingEntry Entry;
    public:
        Tracker() = default;
        Tracker(const Tracker& arg) = default;
        Tracker(Tracker&& arg);

        Tracker& operator=(const Tracker& arg) = default;
        Tracker& operator=(Tracker&& arg);

        ID Add(void* add, bool section);
        ID Add(void* add, bool section, ID id);
        void AddLookahead(void** position);

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

        typedef TrackingLookaheadGroup LookaheadGroup;
        typedef std::unordered_map<ID, LookaheadGroup> LookaheadMap;

        typedef std::unordered_map<const void*, iterator> Checker;
        typedef std::unordered_map<const void*, iterator> Section;
    private:
        Map map;
        Checker checker;

        LookaheadMap lookaheadMap;

        Section section;
    private:
        bool IsTracking(const void* obj) const;
    private:
        void AddLookahead(void** position, ID id);
        LookaheadMap::iterator FindLookaheadList(ID id);
        void LinkLookaheads(void* toObj, ID id);
    private:
        ID NextID() const;
    };
}