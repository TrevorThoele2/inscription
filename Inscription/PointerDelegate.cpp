#include "PointerDelegate.h"

#include "PointerSpecialID.h"

namespace Inscription
{
    PointerDelegate::PointerDelegate() : nextID(PointerSpecialID::ID_START)
    {}

    PointerDelegate::~PointerDelegate()
    {}
}