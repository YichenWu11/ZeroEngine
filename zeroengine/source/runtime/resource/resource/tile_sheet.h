#pragma once

#include "runtime/core/util/json_serializer.h"
#include "runtime/resource/resource.h"

namespace Zero {
    template <>
    class Resource<ResourceType::TileSheet> final : public IResource {
        struct TileSheetDesc {
            std::string tex_desc_path;
            uint16_t    row{1};
            uint16_t    col{1};
        };

    public:
        struct ResLoader final {
            using res_type = Scope<Resource<ResourceType::TileSheet>>;

            res_type operator()(const std::filesystem::path& path) const;
        };

    public:
        Resource(std::string_view name);
        Resource(std::string_view name, const TileSheetDesc& desc);

        bool validate() const { return m_is_valid; }

        uint16_t getRow() const { return m_row; }
        uint16_t getCol() const { return m_col; }

    private:
        uint16_t    m_row;
        uint16_t    m_col;
        std::string m_upper_tex;
    };

    class Tile {
    public:
        Tile() = default;

        Tile(Resource<ResourceType::TileSheet>* tile_sheet,
             const Vector2&                     coords,
             const Vector2&                     tile_size = {1.0f, 1.0f});

        Resource<ResourceType::TileSheet>* getTileSheet() const { return m_tile_sheet; }
        std::string_view                   getTileName() const { return m_tile_name; }

    private:
        bool                               m_is_valid{false};
        std::string                        m_tile_name;
        Resource<ResourceType::TileSheet>* m_tile_sheet;
    };
} // namespace Zero