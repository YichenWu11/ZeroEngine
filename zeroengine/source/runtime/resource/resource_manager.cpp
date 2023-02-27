#include <entt/entt.hpp>

#include "runtime/resource/resource_manager.h"

namespace Zero {
    template <ResourceType _Ty>
    struct TResourceManager : public IResourceManager {
        TResourceManager()  = default;
        ~TResourceManager() = default;

        void tick() override {
            if (update)
                deprecates.clear();
            update = false;
        }

        void add(Scope<IResource>&& resource, size_t uuid) override {
            if (!resource_lookup.contains(uuid)) {
                resource_lookup.emplace(uuid, resources.size());
                resources.emplace_back(Scope<Resource<_Ty>>(dynamic_cast<Resource<_Ty>*>(resource.release())));
            }
            else
                LOG_WARN("try to add a resource whose uuid has been exsited!!!");
        }

        IResource* get(size_t uuid) override {
            ZE_ASSERT(resource_lookup.contains(uuid), "try to get a resource whose uuid does not exsit!!!");
            return resources[resource_lookup[uuid]].get();
        }

        std::string_view getName(size_t uuid) override {
            return has(uuid) ? resources[resource_lookup[uuid]]->getName() : "";
        }

        bool has(size_t uuid) override {
            return resource_lookup.contains(uuid);
        }

        size_t index(size_t uuid) override {
            return (has(uuid)) ? resource_lookup[uuid] : ~0U;
        }

        void erase(size_t uuid) override {
            if (resource_lookup.contains(uuid)) {
                size_t last_uuid = ~0U;
                if (resources.size() > 1) {
                    for (auto& [uuid_, index] : resource_lookup) {
                        if (index + 1 == resource_lookup.size()) {
                            last_uuid = uuid_;
                            break;
                        }
                    }

                    resource_lookup[last_uuid] = resource_lookup[uuid];
                    std::swap(resources.back(), resources[resource_lookup[uuid]]);
                }

                deprecates.emplace_back(std::move(resources.back()));
                resources.pop_back();
                resource_lookup.erase(uuid);
                update = true;
            }
        }

        uint32_t size() override {
            return resources.size();
        }

        std::unordered_map<size_t, size_t> resource_lookup;
        std::vector<Scope<Resource<_Ty>>>  resources;

        std::vector<Scope<Resource<_Ty>>> deprecates;

        bool update = false;
    };

    ResourceManager::ResourceManager() {
        m_managers.emplace(ResourceType::Texture, CreateScope<TResourceManager<ResourceType::Texture>>());
        m_managers.emplace(ResourceType::Mesh, CreateScope<TResourceManager<ResourceType::Mesh>>());
        m_managers.emplace(ResourceType::Animation, CreateScope<TResourceManager<ResourceType::Animation>>());
    }

    void ResourceManager::tick() {
        for (auto& [type, manager] : m_managers) {
            manager->tick();
        }
    }

    void ResourceManager::add(ResourceType type, Scope<IResource>&& resource) {
        size_t uuid = resource->getUUID();
        m_managers.at(type)->add(std::move(resource), uuid);
    }

    IResource* ResourceManager::get(ResourceType type, size_t uuid) {
        return m_managers.at(type)->get(uuid);
    }

    bool ResourceManager::has(ResourceType type, size_t uuid) {
        return m_managers.at(type)->has(uuid);
    }

    size_t ResourceManager::index(ResourceType type, size_t uuid) {
        return m_managers.at(type)->index(uuid);
    }

    void ResourceManager::erase(ResourceType type, size_t uuid) {
        m_managers.at(type)->erase(uuid);
    }

    uint32_t ResourceManager::size(ResourceType type) {
        return m_managers.at(type)->size();
    }
} // namespace Zero
