#pragma once

// runtime included
#include <imgui.h>

#include "runtime/core/common/application.h"
#include "runtime/core/common/layer.h"

#include "runtime/core/util/time_step.h"

#include "runtime/function/gui/imgui_layer.h"
#include "runtime/function/input/input_system.h"

#include "runtime/function/table/mesh_table.h"
#include "runtime/function/table/texture_table.h"

#include "runtime/function/render/camera_system/orthographics_camera_controller.h"
#include "runtime/function/render/render_system/render_command.h"
#include "runtime/function/render/render_system/renderer.h"
#include "runtime/function/render/render_system/renderer_2d.h"
#include "runtime/function/render/render_system/subtexture_2d.h"

#include "runtime/function/scene/components.h"
#include "runtime/function/scene/entity.h"
#include "runtime/function/scene/scene.h"
