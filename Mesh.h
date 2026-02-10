#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include "Vertex.h"
#include <string>

class Mesh
{
public:
	Mesh(Vertex* vertices, unsigned int* indices, size_t vertexCount, size_t indexCount);
	Mesh(const std::wstring& objFile);

	void CreateBuffer(Vertex* vertices, unsigned int* indices, size_t vertexCount, size_t indexCount);
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
	int GetIndexCount();
	int GetVertexCount();
	~Mesh();

private:
	// Buffer views and buffers
	D3D12_VERTEX_BUFFER_VIEW vbView;
	D3D12_INDEX_BUFFER_VIEW ibView;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
	
	int indexCount;
	int vertexCount;

	// Helper function for calculating tangents
	void CalculateTangents(Vertex* vertices, size_t vertexCount, unsigned int* indices, size_t indexCount);
};

