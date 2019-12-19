#pragma once

#include <typeindex>
#include <unordered_map>

namespace Inscription
{
    class Archive
    {
    public:
        Archive(const Archive& arg) = delete;
        Archive& operator=(const Archive& arg) = delete;

        virtual ~Archive() = 0;

        template<class T>
        void EmplaceUserContext(T* userContext);
        template<class T>
        void RemoveUserContext();
        template<class T>
        [[nodiscard]] T* UserContext() const;
    protected:
        Archive() = default;
        Archive(Archive&& arg) noexcept;
        Archive& operator=(Archive&& arg) noexcept;
    private:
        std::unordered_map<std::type_index, void*> userContexts;
    };

    template<class T>
    void Archive::EmplaceUserContext(T* userContext)
    {
        userContexts.emplace(typeid(T), userContext);
    }

    template<class T>
    void Archive::RemoveUserContext()
    {
        userContexts.erase(typeid(T));
    }

    template<class T>
    T* Archive::UserContext() const
    {
        auto found = userContexts.find(typeid(T));
        if (found == userContexts.end())
            return nullptr;

        return reinterpret_cast<T*>(found->second);
    }
}
