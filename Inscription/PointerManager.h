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
    class TrackerMap;
    template<class ScribeT>
    class PointerOutput;
    template<class ScribeT>
    class PointerInput;
    template<class ScribeT>
    class RegisteredTypes;

    template<class ScribeT>
    class PointerManager
    {
    public:
        typedef RegisteredTypes<ScribeT> RegisteredTypesT;
    public:
        PointerManager(Direction direction, RegisteredTypesT& registeredTypes);
        PointerManager(PointerManager&& arg);

        PointerManager& operator=(PointerManager&& arg);

        template<class T>
        void HandleOwning(T*& obj, ScribeT& scribe, TrackerMap& trackers);
        template<class T>
        void HandleUnowning(T*& obj, ScribeT& scribe, TrackerMap& trackers);
    private:
        typedef PointerOutput<ScribeT> OutputDelegate;
        typedef PointerInput<ScribeT> InputDelegate;

        std::unique_ptr<PointerDelegate> delegate;
        Direction direction;
    private:
        RegisteredTypesT* registeredTypes;
    private:
        PointerManager(const PointerManager& arg) = delete;
        PointerManager& operator=(const PointerManager& arg) = delete;
    private:
        void Setup(Direction direction);

        template<class T>
        void Add(const ClassName& name);
    private:
        template<class ScribeU>
        friend class RegisteredTypes;
    };
}