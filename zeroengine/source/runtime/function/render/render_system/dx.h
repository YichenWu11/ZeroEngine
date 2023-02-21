#pragma once

#include <CDX12/CDX12.h>

using Microsoft::WRL::ComPtr;

namespace Zero {
    using DXViewPort = CD3DX12_VIEWPORT;
    using DXRect     = CD3DX12_RECT;

    using DXRawDescHeapPtr = ID3D12DescriptorHeap*;
    using DXRawDevicePtr   = ID3D12Device*;
    using DXRawCmdQueuePtr = ID3D12CommandQueue*;

    using DXFactory          = ComPtr<IDXGIFactory4>;
    using DXSwapChain        = ComPtr<IDXGISwapChain3>;
    using DXCommandSignature = ComPtr<ID3D12CommandSignature>;
    using DXFence            = ComPtr<ID3D12Fence>;
    using DXDebugDevice      = ComPtr<ID3D12DebugDevice>;

    using DXDevice       = Chen::CDX12::Device;
    using DXCommandQueue = Chen::CDX12::CmdQueue;
    using DXCommandList  = Chen::CDX12::GCmdList;

    using DXPSOMngr      = Chen::CDX12::PSOManager;
    using DXFrameResMngr = Chen::CDX12::FrameResourceMngr;

    using DXResStateTracker = Chen::CDX12::ResourceStateTracker;
    using DXBindProperty    = Chen::CDX12::BindProperty;
} // namespace Zero
