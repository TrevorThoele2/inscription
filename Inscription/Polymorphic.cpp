
#include "Polymorphic.h"
#include "Scribe.h"

namespace inscription
{
    PolymorphicTypeNotFoundException::PolymorphicTypeNotFoundException() : Exception("Class type not found in the polymorphic handler.")
    {}

    void PolymorphicManagerRegistration::Fill(PolymorphicManager &manager, Scribe &scribe)
    {
        auto &types = Instance().types;
        for (auto &loop : types)
            loop->Fill(manager, scribe);
    }

    PolymorphicManagerRegistration& PolymorphicManagerRegistration::Instance()
    {
        static PolymorphicManagerRegistration instance;
        return instance;
    }

    PolymorphicManagerRegistration::Base::~Base()
    {}

    PolymorphicManager::PolymorphicManager(Direction direction) : direction(direction)
    {
        Setup(direction);
    }

    PolymorphicManager::PolymorphicManager(PolymorphicManager &&arg) : direction(arg.direction), implementation(std::move(arg.implementation))
    {}

    PolymorphicManager& PolymorphicManager::operator=(PolymorphicManager &&arg)
    {
        direction = arg.direction;
        implementation = std::move(arg.implementation);
        return *this;
    }

    void PolymorphicManager::Fill(Scribe &scribe)
    {
        PolymorphicManagerRegistration::Fill(*this, scribe);
    }

    PolymorphicManager::Impl::~Impl()
    {}

    PolymorphicManager::OutputImpl::EntryBase::EntryBase(const ClassName &name) : id(SpecialIDs::NULLPTR), name(name)
    {}

    PolymorphicManager::OutputImpl::EntryBase::~EntryBase()
    {}

    const PolymorphicManager::ClassName& PolymorphicManager::OutputImpl::EntryBase::GetName() const
    {
        return name;
    }

    void PolymorphicManager::OutputImpl::EntryBase::SaveID(Scribe &scribe)
    {
        scribe.WriteNumericImplementation(id);
    }

    PolymorphicManager::InputImpl::EntryBase::EntryBase(const ClassName &name) : name(name)
    {}

    PolymorphicManager::InputImpl::EntryBase::~EntryBase()
    {}

    const PolymorphicManager::ClassName& PolymorphicManager::InputImpl::EntryBase::GetName() const
    {
        return name;
    }

    void PolymorphicManager::Setup(Direction direction)
    {
        switch (direction)
        {
        case Direction::OUTPUT:
            implementation.reset(new OutputImpl());
            break;
        case Direction::INPUT:
            implementation.reset(new InputImpl());
            break;
        }
    }
}