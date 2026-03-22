#pragma once
#include <DirectXMath.h>
#include "Lights.h"

// Struct that defines basic data to pass into the vertex shader
struct VertexShaderExternalData
{
	DirectX::XMFLOAT4X4 m4World;
	DirectX::XMFLOAT4X4 m4View;
	DirectX::XMFLOAT4X4 m4Projection;
	DirectX::XMFLOAT4X4 m4WorldInvTranspose;
};

// Struct that defines basic data that will be sent to the pixel shader
struct PixelShaderExternalData
{
	unsigned int albedo;
	unsigned int normal;
	unsigned int metal;
	unsigned int roughness;
	DirectX::XMFLOAT2 uvScale;
	DirectX::XMFLOAT2 uvOffset;
	DirectX::XMFLOAT3 cameraPosition;
	unsigned int lightCount;
	Light lights[3];
};

// Root constants for bindless resources
struct RayTracingDrawData
{
	unsigned int SceneDataConstantBufferIndex;
	unsigned int EntityDataDescriptorIndex;
	unsigned int SceneTLASDescriptorIndex;
	unsigned int OutputUAVDescriptorIndex;
};

// Overall scene data for ray tracing
struct RaytracingSceneData
{
	DirectX::XMFLOAT4X4 InverseViewProjection;
	DirectX::XMFLOAT3 CameraPosition;
	float pad;
};

// Per-entity information (geom, material, etc.)
// - Multiple sets of these will be stored in a structured buffer
// - Materials *could* be separated out into their own buffer
//   to cut down on data repition
struct RayTracingEntityData
{
	DirectX::XMFLOAT4 Color;
	unsigned int VertexBufferDescriptorIndex;
	unsigned int IndexBufferDescriptorIndex;
	float pad[2];
};