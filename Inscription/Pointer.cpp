
#include "Pointer.h"

#include "PointerOutput.h"
#include "PointerInput.h"

#include "Scribe.h"

namespace Inscription
{
    PointerManager::PointerManager(Direction direction) : direction(direction)
    {
        Setup(direction);
    }

    PointerManager::PointerManager(PointerManager &&arg) : direction(arg.direction), delegate(std::move(arg.delegate))
    {}

    PointerManager& PointerManager::operator=(PointerManager &&arg)
    {
        direction = arg.direction;
        delegate = std::move(arg.delegate);
        return *this;
    }

    void PointerManager::Fill(Scribe &scribe)
    {
        RegisteredTypes::PushToPolymorphic(*this, scribe);
    }

    void PointerManager::Setup(Direction direction)
    {
        switch (direction)
        {
        case Direction::OUTPUT:
            delegate.reset(new PointerOutput());
            break;
        case Direction::INPUT:
            delegate.reset(new PointerInput());
            break;
        }
    }
}