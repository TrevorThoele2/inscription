
#include "PointerDelegate.h"

#include "PointerSpecialIDs.h"

namespace Inscription
{
    PointerDelegate::PointerDelegate() : nextID(PointerSpecialIDs::ID_START) {}

    PointerDelegate::~PointerDelegate()
    {}
}