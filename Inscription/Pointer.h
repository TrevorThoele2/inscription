
#pragma once

#include <memory>
#include <functional>
#include <list>
#include <unordered_map>
#include <vector>

#include "PointerDelegate.h"
#include "PolymorphicID.h"

#include "Tracking.h"
#include "ClassNameResolver.h"
#include "Function.h"
#include "Direction.h"

#include "String.h"

#include "Exception.h"

namespace Inscription
{
    class PointerManager
    {
    public:
        PointerManager(Direction direction);
        PointerManager(PointerManager &&arg);
        PointerManager& operator=(PointerManager &&arg);
        void Fill(Scribe &scribe);

        template<class T>
        void HandleOwning(T*& obj, Scribe& scribe, TrackerGroup& trackers);
        template<class T>
        void HandleUnowning(T*& obj, Scribe& scribe, TrackerGroup& trackers);
    private:
        std::unique_ptr<PointerDelegate> delegate;
        Direction direction;
    private:
        PointerManager(const PointerManager &arg) = delete;
        PointerManager& operator=(const PointerManager &arg) = delete;
    private:
        void Setup(Direction direction);

        template<class T>
        void Add(const ClassName &name);
    private:
        friend class RegisteredTypes;
    };
}