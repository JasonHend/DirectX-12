#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <string>
#include <wrl/client.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

namespace Graphics
{
	// --- CONSTANTS ---
	const unsigned int NumBackBuffers = 2;
	const unsigned int MaxTextureDescriptors = 100; // Max amount of SRVs

	// --- GLOBAL VARS ---

	// Primary D3D12 API objects
	inline Microsoft::WRL::ComPtr<ID3D12Device>   Device;
	inline Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;

	// Command submission
	inline Microsoft::WRL::ComPtr<ID3D12CommandAllocator>	 CommandAllocator;
	inline Microsoft::WRL::ComPtr<ID3D12CommandQueue>		 CommandQueue;
	inline Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList;

	// Rendering buffers and descriptors
	inline Microsoft::WRL::ComPtr<ID3D12Resource>		BackBuffers[NumBackBuffers];
	inline Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVHeap;
	inline D3D12_CPU_DESCRIPTOR_HANDLE					RTVHandles[NumBackBuffers];

	inline Microsoft::WRL::ComPtr<ID3D12Resource>		DepthBuffer;
	inline Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DSVHeap;
	inline D3D12_CPU_DESCRIPTOR_HANDLE					DSVHandle{};

	// Basic CPU/GPU synchronization
	inline Microsoft::WRL::ComPtr<ID3D12Fence> WaitFence;
	inline HANDLE							   WaitFenceEvent = 0;
	inline UINT64							   WaitFenceCounter = 0;

	// Debug Layer
	inline Microsoft::WRL::ComPtr<ID3D12InfoQueue> InfoQueue;

	// Maximum number of constant buffers, assuming each buffer
	// is 256 bytes or less. Larger buffers are fine, but will
	// result in fewer buffers in use at any time
	const unsigned int MaxConstantBuffers = 1000;

	inline Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CBVSRVDescriptorHeap;
	inline Microsoft::WRL::ComPtr<ID3D12Resource> CBUploadHeap;

	// --- FUNCTIONS ---

	// Getters
	bool VsyncState();
	std::wstring APIName();
	unsigned int SwapChainIndex();

	// General functions
	HRESULT Initialize(unsigned int windowWidth, unsigned int windowHeight, HWND windowHandle, bool vsyncIfPossible);
	void ShutDown();
	void ResizeBuffers(unsigned int width, unsigned int height);
	void AdvanceSwapChainIndex();

	// Helper for accessing a certain descriptor index
	unsigned int GetDescriptorIndex(D3D12_GPU_DESCRIPTOR_HANDLE handle);

	// Resource creation and utiliziation
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateStaticBuffer(
		size_t dataStride, size_t dataCount, void* data);

	D3D12_GPU_DESCRIPTOR_HANDLE FillNextConstantBufferAndGetGPUDescriptorHandle(
		void* data,
		unsigned int dataSizeInBytes);
	
	unsigned int LoadTexture(const wchar_t* file, bool generateMips = true);

	// Cubemap loader
	unsigned int CreateCubemap(
		const wchar_t* right,
		const wchar_t* left,
		const wchar_t* up,
		const wchar_t* down,
		const wchar_t* front,
		const wchar_t* back);

	// Descriptor heap
	void ReserveDescriptorHeapSlot(
		D3D12_CPU_DESCRIPTOR_HANDLE* reservedCPUHandle,
		D3D12_GPU_DESCRIPTOR_HANDLE* reservedGPUHandle);

	// Command list and synchronization
	void ResetAllocatorAndCommandList();
	void CloseAndExecuteCommandList();
	void WaitForGPU();

	// Debug Layer
	void PrintDebugMessages();
}