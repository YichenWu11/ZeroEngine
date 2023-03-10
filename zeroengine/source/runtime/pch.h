#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// if windows
#include <Windows.h>
#include <WindowsX.h>

#include <D3Dcompiler.h>
#include <DirectXCollision.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <dxgi1_6.h>
#include <shellapi.h>
#include <wrl.h>

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <span>
#include <utility>

#include <array>
#include <fstream>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <variant>
#include <vector>


#include <SimpleMath.h>

#include <box2d/box2d.h>
#include <entt/entt.hpp>

#include "runtime/core/base/core.h"
#include "runtime/core/log/log_system.h"
#include "runtime/function/debug/profiler.h"

using namespace DirectX::SimpleMath;

namespace Zero {
    using EntityIdType = ENTT_ID_TYPE;
}
