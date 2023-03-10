#include "runtime/function/scene/components.h"
#include "runtime/core/base/application.h"

namespace Zero {
    TileComponent::TileComponent(std::string_view tile_sheet_name) {
        tile_sheet = Zero::Application::get().getResourceMngr()->get<Zero::ResourceType::TileSheet>(std::string(tile_sheet_name));
    }
} // namespace Zero