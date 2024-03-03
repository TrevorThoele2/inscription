#pragma once

#include "PointerManager.h"
#include "PointerOutput.h"
#include "PointerInput.h"

namespace Inscription
{
    template<class ScribeT>
    PointerManager<ScribeT>::PointerManager(Direction direction) : direction(direction)
    {
        Setup(direction);
    }

    template<class ScribeT>
    PointerManager<ScribeT>::PointerManager(PointerManager&& arg) : direction(arg.direction), delegate(std::move(arg.delegate))
    {}

    template<class ScribeT>
    PointerManager<ScribeT>& PointerManager<ScribeT>::operator=(PointerManager&& arg)
    {
        direction = arg.direction;
        delegate = std::move(arg.delegate);
        return *this;
    }

    template<class ScribeT>
    template<class T>
    void PointerManager<ScribeT>::HandleOwning(T*& obj, ScribeT& scribe, TrackerMap& trackers)
    {
        switch (direction)
        {
        case Direction::OUTPUT:
            static_cast<OutputDelegate*>(delegate.get())->HandleOwning(obj, *scribe.AsOutput(), trackers);
            break;
        case Direction::INPUT:
            static_cast<InputDelegate*>(delegate.get())->HandleOwning(obj, *scribe.AsInput(), trackers);
            break;
        }
    }

    template<class ScribeT>
    template<class T>
    void PointerManager<ScribeT>::HandleUnowning(T*& obj, ScribeT& scribe, TrackerMap& trackers)
    {
        switch (direction)
        {
        case Direction::OUTPUT:
            static_cast<OutputDelegate*>(delegate.get())->HandleUnowning(obj, *scribe.AsOutput(), trackers);
            break;
        case Direction::INPUT:
            static_cast<InputDelegate*>(delegate.get())->HandleUnowning(obj, *scribe.AsInput(), trackers);
            break;
        }
    }

    template<class ScribeT>
    template<class T>
    void PointerManager<ScribeT>::Register(const ClassName& name)
    {
        switch (direction)
        {
        case Direction::OUTPUT:
            static_cast<OutputDelegate*>(delegate.get())->Push<T>(name);
            break;
        case Direction::INPUT:
            static_cast<InputDelegate*>(delegate.get())->Push<T>(name);
            break;
        }
    }

    template<class ScribeT>
    void PointerManager<ScribeT>::Setup(Direction direction)
    {
        switch (direction)
        {
        case Direction::OUTPUT:
            delegate.reset(new OutputDelegate());
            break;
        case Direction::INPUT:
            delegate.reset(new InputDelegate());
            break;
        }
    }
}