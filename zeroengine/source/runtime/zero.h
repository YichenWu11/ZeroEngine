#pragma once

// runtime included
#include <imgui.h>

#include "runtime/core/base/application.h"
#include "runtime/core/base/layer.h"

#include "runtime/core/util/file_dialog.h"
#include "runtime/core/util/time_step.h"

#include "runtime/function/gui/imgui_layer.h"
#include "runtime/function/input/input_system.h"

#include "runtime/function/pool/mesh_pool.h"
#include "runtime/function/pool/texture_pool.h"

#include "runtime/function/render/camera_system/orthographics_camera_controller.h"
#include "runtime/function/render/render_system/render_command.h"
#include "runtime/function/render/render_system/renderer.h"
#include "runtime/function/render/render_system/renderer_2d.h"
#include "runtime/function/render/render_system/subtexture_2d.h"

#include "runtime/function/scene/components.h"
#include "runtime/function/scene/entity.h"
#include "runtime/function/scene/scene.h"
#include "runtime/function/scene/scriptable_entity.h"
