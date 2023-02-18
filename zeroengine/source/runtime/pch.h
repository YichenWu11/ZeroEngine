#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// if windows
#include <Windows.h>
#include <WindowsX.h>

#include <CDX12/_deps/d3dx12.h>
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
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <variant>
#include <vector>

#include <SimpleMath.h>

#include "runtime/core/base/core.h"
#include "runtime/core/log/log_system.h"
#include "runtime/function/debug/profiler.h"