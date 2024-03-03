
#pragma once

#include <memory>
#include <list>
#include <unordered_map>
#include <vector>

#include "PolymorphicID.h"

#include "Tracking.h"
#include "ClassName.h"
#include "Function.h"
#include "Direction.h"

#include "String.h"

#include "Exception.h"

namespace inscription
{
    class PolymorphicTypeNotFoundException : public Exception
    {
    public:
        PolymorphicTypeNotFoundException();
    };

    class Scribe;
    class PolymorphicManager;
    class PolymorphicManagerRegistration
    {
    public:
        template<class T>
        static void Register();
        static void Fill(PolymorphicManager &manager, Scribe &scribe);
    private:
        class Base
        {
        public:
            virtual ~Base() = 0;
            virtual void Fill(PolymorphicManager &manager, Scribe &scribe) = 0;
        };

        template<class T>
        class Derived : public Base
        {
        public:
            static Derived instance;
            Derived();
            // Definition is after PolymorphicManager's
            void Fill(PolymorphicManager &manager, Scribe &scribe) override;
        };
    private:
        std::vector<Base*> types;

        static PolymorphicManagerRegistration& Instance();
    };

    class PolymorphicManager
    {
    public:
        PolymorphicManager(Direction direction);
        PolymorphicManager(PolymorphicManager &&arg);
        PolymorphicManager& operator=(PolymorphicManager &&arg);
        void Fill(Scribe &scribe);

        template<class T>
        void HandleObject(T *&obj, Scribe &scribe, TrackerGroup &trackers);
        template<class T>
        void HandleObjectNoConstruct(T *&obj, Scribe &scribe, TrackerGroup &trackers);
        template<class T>
        static void Register();
    private:
        enum SpecialIDs : unsigned int
        {
            NULLPTR = 0,
            HANDLE_NORMALLY = 1,
            ID_START = 2
        };

        typedef ClassNameContainer::Name ClassName;

        // Implementation classes
        class Impl
        {
        public:
            Impl() : nextID(SpecialIDs::ID_START) {}
            virtual ~Impl() = 0;
        protected:
            PolymorphicID nextID;
        };

        class OutputImpl : public Impl
        {
        public:
            template<class T>
            void HandleObject(T *&obj, Scribe &scribe, TrackerGroup &trackers);
            template<class T>
            void HandleObjectNoConstruct(T *&obj, Scribe &scribe, TrackerGroup &trackers);
            template<class T>
            void Push(const ClassName &name);
        private:
            class EntryBase
            {
            public:
                PolymorphicID id;

                EntryBase(const ClassName &name);
                virtual ~EntryBase() = 0;

                const ClassName& GetName() const;
                void SaveID(Scribe &scribe);

                virtual void SaveObject(const void *obj, Scribe &scribe) = 0;
                virtual void EnableTracking(TrackerGroup &trackers) = 0;
            private:
                ClassName name;
            };

            typedef std::unique_ptr<EntryBase> EntryPtr;

            template<class T>
            class Entry : public EntryBase
            {
            public:
                Entry(const ClassName &name);
                void SaveObject(const void *obj, Scribe &scribe) override;
                void EnableTracking(TrackerGroup &trackers) override;
            };
        private:
            typedef std::list<EntryPtr> List;
            List list;
            std::unordered_map<std::type_index, List::iterator> types;

            template<class T, typename std::enable_if<!std::is_abstract<T>::value, int>::type = 0>
            void HandleObjectSpecial(Scribe &scribe, TrackerGroup &trackers, T *obj);
            template<class T, typename std::enable_if<std::is_abstract<T>::value, int>::type = 0>
            void HandleObjectSpecial(Scribe &scribe, TrackerGroup &trackers, T *obj) {}
        };

        class InputImpl : public Impl
        {
        public:
            template<class T>
            void HandleObject(T *&obj, Scribe &scribe, TrackerGroup &trackers);
            template<class T>
            void HandleObjectNoConstruct(T *&obj, Scribe &scribe, TrackerGroup &trackers);
            template<class T>
            void Push(const ClassName &name);
        private:
            class EntryBase
            {
            public:
                EntryBase(const ClassName &name);
                virtual ~EntryBase() = 0;

                const ClassName& GetName() const;

                virtual void* GetObject(Scribe &scribe) = 0;
                virtual void* GetObjectNoTrack(Scribe &scribe) = 0;
                virtual void EnableTracking(TrackerGroup &trackers) = 0;
            private:
                ClassName name;
            };

            typedef std::unique_ptr<EntryBase> EntryPtr;

            template<class T>
            class Entry : public EntryBase
            {
            public:
                Entry(const ClassName &name);
                void* GetObject(Scribe &scribe) override;
                void* GetObjectNoTrack(Scribe &scribe) override;
                void EnableTracking(TrackerGroup &trackers) override;
            };
        private:
            typedef std::list<EntryPtr> List;
            List list;

            std::unordered_map<PolymorphicID, ClassName> ids;
            std::unordered_map<ClassName, List::iterator> classNames;

            template<class T, typename std::enable_if<!std::is_abstract<T>::value, int>::type = 0>
            bool HandleObjectSpecial(Scribe &scribe, PolymorphicID id, T *&obj);
            template<class T, typename std::enable_if<std::is_abstract<T>::value, int>::type = 0>
            bool HandleObjectSpecial(Scribe &scribe, PolymorphicID id, T *&obj) { return false; }
        };
    private:
        std::unique_ptr<Impl> implementation;
        Direction direction;

        PolymorphicManager(const PolymorphicManager &arg) = delete;
        PolymorphicManager& operator=(const PolymorphicManager &arg) = delete;

        void Setup(Direction direction);

        template<class T>
        void Add(const ClassName &name);

        friend PolymorphicManagerRegistration;
    };
}