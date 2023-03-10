#pragma once

namespace Zero {
    enum class ResourceType {
        Unknown,
        Texture,
        Mesh,
        TileSheet,
        Animation,
    };

    class IResource {
    public:
        explicit IResource(std::string_view name);
        virtual ~IResource() = default;

        std::string_view getName() const;
        size_t           getUUID() const;

        virtual bool validate() const { return m_is_valid; }

    protected:
        std::string m_name;
        bool        m_is_valid{false};
    };

    template <ResourceType Type>
    class Resource : public IResource {
    public:
        Resource() = default;

        virtual ~Resource() override = default;
    };

} // namespace Zero
