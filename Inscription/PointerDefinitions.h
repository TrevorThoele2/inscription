#pragma once

#include "PointerManager.h"
#include "PointerOutput.h"
#include "PointerInput.h"

namespace Inscription
{
    template<class T>
    void PointerManager::HandleOwning(T*& obj, BinaryScribe& scribe, TrackerMap& trackers)
    {
        switch (direction)
        {
        case Direction::OUTPUT:
            static_cast<PointerOutput*>(delegate.get())->HandleOwning(obj, scribe, trackers);
            break;
        case Direction::INPUT:
            static_cast<PointerInput*>(delegate.get())->HandleOwning(obj, scribe, trackers);
            break;
        }
    }

    template<class T>
    void PointerManager::HandleUnowning(T*& obj, BinaryScribe& scribe, TrackerMap& trackers)
    {
        switch (direction)
        {
        case Direction::OUTPUT:
            static_cast<PointerOutput*>(delegate.get())->HandleUnowning(obj, scribe, trackers);
            break;
        case Direction::INPUT:
            static_cast<PointerInput*>(delegate.get())->HandleUnowning(obj, scribe, trackers);
            break;
        }
    }

    template<class T>
    void PointerManager::Add(const ClassName &name)
    {
        switch (direction)
        {
        case Direction::OUTPUT:
            static_cast<PointerOutput*>(delegate.get())->Push<T>(name);
            break;
        case Direction::INPUT:
            static_cast<PointerInput*>(delegate.get())->Push<T>(name);
            break;
        }
    }
}