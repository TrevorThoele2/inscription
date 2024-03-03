
#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <typeindex>

#include "Version.h"
#include "Enum.h"

namespace inscription
{
    class Scribe;
    class ClassNameContainer
    {
    public:
        typedef std::string Name;

        enum class Op : unsigned char
        {
            ANY,
            EQUAL,
            LESSER,
            GREATER
        };
    public:
        ClassNameContainer(Name &&defaultName) : defaultName(std::move(defaultName)) {}

        // Pass in a function to initialize this container
        template<class Func>
        ClassNameContainer(Name &&defaultName, Func func) : defaultName(std::move(defaultName))
        { func(*this); }

        // Use this to explicitly add a name for a specific scribe/version
        template<class ScribeT>
        void AddName(Name &&name, Op op, Version version);
        const Name& GetName(Scribe &scribe) const;
    private:
        class Entry
        {
        public:
            Name name;
            Version version;

            Entry(Name &&name, Version version) : name(std::move(name)), version(version) {}
        };
    private:
        typedef std::unordered_multimap<Op, Entry, HashEnum> Operations;
        typedef std::unordered_map<std::type_index, Operations> ScribeTypes;
        ScribeTypes entries;

        Name defaultName;
    };

    template<class ScribeT>
    void ClassNameContainer::AddName(Name &&name, Op op, Version version)
    {
        auto scribeT = std::type_index(typeid(ScribeT));
        auto found = entries.find(scribeT);
        if (found == entries.end())
            entries.emplace(scribeT, Operations()).first->second.emplace(op, Entry(std::move(name), version));
        else
            found->second.emplace(op, Entry(std::move(name), version));
    }
}