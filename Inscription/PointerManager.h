#pragma once

#include <memory>
#include <functional>
#include <list>
#include <unordered_map>
#include <vector>

#include "PointerDelegate.h"
#include "PolymorphicID.h"

#include "ClassNameResolver.h"
#include "Direction.h"

#include "String.h"

#include "Exception.h"

namespace Inscription
{
    class BinaryScribe;
    class TrackerMap;

    class PointerManager
    {
    public:
        PointerManager(Direction direction);
        PointerManager(PointerManager&& arg);

        PointerManager& operator=(PointerManager&& arg);

        void Fill(BinaryScribe& scribe);

        template<class T>
        void HandleOwning(T*& obj, BinaryScribe& scribe, TrackerMap& trackers);
        template<class T>
        void HandleUnowning(T*& obj, BinaryScribe& scribe, TrackerMap& trackers);
    private:
        std::unique_ptr<PointerDelegate> delegate;
        Direction direction;
    private:
        PointerManager(const PointerManager& arg) = delete;
        PointerManager& operator=(const PointerManager& arg) = delete;
    private:
        void Setup(Direction direction);

        template<class T>
        void Add(const ClassName& name);
    private:
        friend class RegisteredTypes;
    };
}