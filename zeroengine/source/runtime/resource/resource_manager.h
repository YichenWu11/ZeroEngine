#pragma once

#include "runtime/resource/mesh.h"
#include "runtime/resource/resource.h"
#include "runtime/resource/texture.h"

namespace Zero {
    struct IResourceManager {
        IResourceManager()          = default;
        virtual ~IResourceManager() = default;

        virtual void tick() = 0;

        virtual void add(Scope<IResource>&& resource, size_t uuid) = 0;

        virtual IResource*       get(size_t uuid)     = 0;
        virtual std::string_view getName(size_t uuid) = 0;

        virtual bool     has(size_t uuid)   = 0;
        virtual size_t   index(size_t uuid) = 0;
        virtual void     erase(size_t uuid) = 0;
        virtual uint32_t size()             = 0;
    };

    class ResourceManager {
    public:
        ResourceManager();
        ~ResourceManager() = default;

        void tick();

        template <ResourceType Type>
        Resource<Type>* get(std::string_view name) {
            return static_cast<Resource<Type>*>(get(Type, Hash(name)));
        }

        template <ResourceType Type>
        bool has(std::string_view name) {
            return has(Type, Hash(name));
        }

        template <ResourceType Type>
        size_t index(std::string_view name) {
            return index(Type, Hash(name));
        }

        template <ResourceType Type>
        void erase(std::string_view path) {
            erase(Type, Hash(path));
        }

        template <ResourceType Type>
        uint32_t size() {
            return size(Type);
        }

        template <ResourceType Type, typename... Args>
        void add(Args&&... args) {
            typename Resource<Type>::ResLoader loader;
            add(Type, loader(std::forward<Args>(args)...));
        }

    private:
        void       add(ResourceType type, Scope<IResource>&& resource);
        IResource* get(ResourceType type, size_t uuid);
        bool       has(ResourceType type, size_t uuid);
        size_t     index(ResourceType type, size_t uuid);
        void       erase(ResourceType type, size_t uuid);
        uint32_t   size(ResourceType type);

    private:
        std::map<ResourceType, Scope<IResourceManager>> m_managers;
    };
} // namespace Zero
