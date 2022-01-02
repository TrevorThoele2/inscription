#pragma once

#include <typeindex>
#include <unordered_map>

namespace Inscription::Format
{
    class Format
    {
    public:
        Format(const Format& arg) = delete;
        Format& operator=(const Format& arg) = delete;

        virtual ~Format() = 0;

        template<class T>
        void EmplaceUserContext(T* userContext);
        template<class T>
        void RemoveUserContext();
        template<class T>
        [[nodiscard]] T* UserContext() const;
    protected:
        Format() = default;
        Format(Format&& arg) noexcept;
        Format& operator=(Format&& arg) noexcept;
    private:
        std::unordered_map<std::type_index, void*> userContexts;
    };

    template<class T>
    void Format::EmplaceUserContext(T* userContext)
    {
        userContexts.emplace(typeid(T), userContext);
    }

    template<class T>
    void Format::RemoveUserContext()
    {
        userContexts.erase(typeid(T));
    }

    template<class T>
    T* Format::UserContext() const
    {
        auto found = userContexts.find(typeid(T));
        if (found == userContexts.end())
            return nullptr;

        return reinterpret_cast<T*>(found->second);
    }
}
