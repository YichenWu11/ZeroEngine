#pragma once

// runtime included
#include <imgui.h>

#include "runtime/core/base/application.h"
#include "runtime/core/base/layer.h"

#include "runtime/core/util/file_dialog.h"
#include "runtime/core/util/time_step.h"

#include "runtime/function/input/input_system.h"

#include "runtime/function/gui/imgui_layer.h"
#include "runtime/function/gui/widgets.h"

#include "runtime/function/render/camera_system/editor_camera.h"
#include "runtime/function/render/camera_system/orthographics_camera_controller.h"

#include "runtime/function/render/render_system/render_command.h"
#include "runtime/function/render/render_system/renderer.h"
#include "runtime/function/render/render_system/renderer_2d.h"
#include "runtime/function/render/render_system/subtexture_2d.h"

#include "runtime/function/scene/components.h"
#include "runtime/function/scene/entity.h"
#include "runtime/function/scene/scene.h"
#include "runtime/function/scene/scene_serializer.h"
#include "runtime/function/scene/scriptable_entity.h"
#include "runtime/function/scene/world.h"

#include "runtime/resource/resource/mesh.h"
#include "runtime/resource/resource/texture.h"
#include "runtime/resource/resource_manager.h"
