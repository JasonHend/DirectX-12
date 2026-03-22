#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include "Vertex.h"
#include <string>

class Mesh
{
public:
	// Structs
	struct MeshRayTracingData
	{
		D3D12_GPU_DESCRIPTOR_HANDLE IndexBufferSRV {};
		D3D12_GPU_DESCRIPTOR_HANDLE VertexBufferSRV {};
		Microsoft::WRL::ComPtr<ID3D12Resource> BLAS;
	};

	Mesh(Vertex* vertices, unsigned int* indices, size_t vertexCount, size_t indexCount);
	Mesh(const std::wstring& objFile);

	void CreateBuffer(Vertex* vertices, unsigned int* indices, size_t vertexCount, size_t indexCount);
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
	Microsoft::WRL::ComPtr<ID3D12Resource> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBuffer();
	int GetIndexCount();
	int GetVertexCount();
	const MeshRayTracingData& GetRayTracingData();
	~Mesh();

private:
	// Buffer views and buffers
	D3D12_VERTEX_BUFFER_VIEW vbView;
	D3D12_INDEX_BUFFER_VIEW ibView;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
	
	int indexCount;
	int vertexCount;

	// Ray tracing variables
	MeshRayTracingData rayTracingData;

	// Helper function for calculating tangents
	void CalculateTangents(Vertex* vertices, size_t vertexCount, unsigned int* indices, size_t indexCount);
};

